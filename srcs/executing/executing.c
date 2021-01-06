/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:03:05 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/06 19:13:34 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/******  set_redirection_fd.c  start ******/
int		set_redirection_fd(t_msh *msh, t_cmd *cmd)
{
	t_list *list;

	list = cmd->redirection_files;
	while(list)
	{
		if (!ft_strcmp(">", list->content))
		{
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd = open(list->content, FLAG_O, OPEN_MODE)) == -1)
				return (print_execute_err(msh->program_name, list->content, strerror(errno)));
		}
		else if (!ft_strcmp(">>", list->content))
		{
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd = open(list->content, FLAG_AO, OPEN_MODE)) == -1)
				return (print_execute_err(msh->program_name, list->content, strerror(errno)));
		}
		else if (!ft_strcmp("<", list->content))
		{
			if (cmd->input_fd != -1)
				close(cmd->input_fd);
			list = list->next;
			if ((cmd->input_fd = open(list->content, FLAG_I, OPEN_MODE)) == -1)
				return (print_execute_err(msh->program_name, list->content, strerror(errno)));
		}
		list = list->next;
	}
	return (SUCCESS);
}
/******    set_redirection_fd.c  end ********/

void			basic_executor(t_msh *msh, t_cmd *cmd)
{
	char	**env;
	char	*temp;
	int		idx;

	if (cmd->args[0] == NULL)
		exit(0);

	if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	
	env = ft_envjoin(msh->env, msh->env_len);
	
	if (msh->path[0] == NULL || ft_strchr(cmd->args[0], '/') != NULL)
	{
		if ((execve(cmd->args[0], cmd->args, env)) == -1)
		{
			print_execute_err(msh->program_name, cmd->args[0], strerror(errno));
		//	if (errno == 13)
		//		exit(126);
			exit(127);
		}
	}
	else
	{
		idx = 0;
		while (msh->path[idx])
		{
			if (!(temp = ft_strjoin3(msh->path[idx], "/", cmd->args[0])))
				exit_print_err(strerror(errno));
			execve(temp, cmd->args, env);
			free(temp);
			idx++;
		}
		print_execute_err(msh->program_name, cmd->args[0], "command not found");
		exit(127);
	}
}


void	create_process(t_msh *msh, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) == -1)
		exit_print_err(strerror(errno));
	if (pid == 0)
		basic_executor(msh, cmd);
	else
	{
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			msh->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			msh->exit_status = 128 + WTERMSIG(status);

		if (cmd->input_fd != -1)
			close(cmd->input_fd);
		if (cmd->output_fd != -1)
			close(cmd->output_fd);
	}
}


/****************   piping start   ********************************************/


int get_cnt_of_pipes(t_cmd *cmd)
{
	int cnt;

	cnt = 0;
	while (cmd)
	{
		if (cmd->type != TYPE_PIPE)
			break;
		cnt++;
		cmd = cmd->next;
	}
	return (cnt);
}

void create_pipes(int *pipes, int cnt_of_pipes)
{
	int i;

	i = 0;
	while (i < cnt_of_pipes)
	{
		if (pipe(pipes + (i * 2)) == -1)
			exit_print_err(strerror(errno));
		i++;
	}
}

void close_pipes(int *pipes, int cnt_of_pipes)
{
	int i;

	i = 0;
	while (i < 2 * cnt_of_pipes)
	{
		close(pipes[i]);
		i++;
	}
}

void all_wait(t_msh *msh, int *cpid, int cnt_of_pipes)
{
	int status;
	int	i;

	i = 0;
	while (i < cnt_of_pipes + 1)
	{
		waitpid(cpid[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		msh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		msh->exit_status = 128 + WTERMSIG(status);
}


t_cmd	*piping(t_msh *msh, t_cmd *cmd)
{
	t_builtin_executor	builtin_executor;
	int cnt_of_pipes;
	int *pipes;
	int *cpid;
	int i;

	cnt_of_pipes = get_cnt_of_pipes(cmd);
	if (!(pipes = malloc(sizeof(int) * (cnt_of_pipes * 2))))
		exit_print_err(strerror(errno));
	if (!(cpid = malloc(sizeof(pid_t) * (cnt_of_pipes + 1))))
		exit_print_err(strerror(errno));

	create_pipes(pipes, cnt_of_pipes);

	i = 0;
	while (i < (cnt_of_pipes + 1))
	{
		if ((cpid[i] = fork()) == 0)
		{

			if (i < cnt_of_pipes)
				dup2(pipes[i * 2 + 1], 1);
			if (i > 0)
				dup2(pipes[(i - 1) * 2], 0);
			close_pipes(pipes, cnt_of_pipes);

			
			builtin_executor  = get_builtin_executor(cmd->args[0]);	
			if (set_redirection_fd(msh, cmd) == ERROR)
			{
				exit(1);
			}

			if (builtin_executor)
			{
				if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
					exit_print_err(strerror(errno));
				if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
					exit_print_err(strerror(errno));
				builtin_executor(msh, cmd);
				exit(msh->exit_status);
			}
			else
			{
				basic_executor(msh, cmd);
			}
		}
		else if (cpid[i] == -1)
		{
			exit_print_err(strerror(errno));
		}
		cmd = cmd->next;
		i++;
	}
	
	close_pipes(pipes, cnt_of_pipes);
	all_wait(msh, cpid, cnt_of_pipes);
	free(pipes);
	free(cpid);
	return (cmd);
}
/****************   piping end   **********************************************/

void	executing(t_msh *msh)
{
	t_builtin_executor	builtin_executor;
	t_cmd		*cmd;
	int stdin_fd;
	int stdout_fd;

	cmd = msh->cmds;
	while (cmd)
	{
		if (cmd->type == TYPE_PIPE)
		{
			cmd = piping(msh, cmd);
			continue ;
		}
		
		builtin_executor = get_builtin_executor(cmd->args[0]);
		

		if (set_redirection_fd(msh, cmd) == ERROR)
		{
			if (cmd->input_fd != -1)
				close(cmd->input_fd);
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			cmd = cmd->next;
			msh->exit_status = 1;
			continue;
		}

		if (builtin_executor)
		{
			if ((stdin_fd = dup(STDIN)) < 0)
				exit_print_err(strerror(errno));
			if ((stdout_fd = dup(STDOUT)) < 0)
				exit_print_err(strerror(errno));

			if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
				exit_print_err(strerror(errno));
			if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
				exit_print_err(strerror(errno));

			builtin_executor(msh, cmd);

			if (dup2(stdin_fd, STDIN) < 0)
				exit_print_err(strerror(errno));
			if (dup2(stdout_fd, STDOUT) < 0)
				exit_print_err(strerror(errno));

			close(stdin_fd);
			close(stdout_fd);
		}
		else
		{
			create_process(msh, cmd);
		}

		cmd = cmd->next;
	}
}
