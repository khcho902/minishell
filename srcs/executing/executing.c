/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:03:05 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/01 17:29:50 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_input_fd(t_cmd *cmd, t_list *list)
{
	while (list)
	{
		if (!ft_strcmp("<", list->content))
		{
			if (cmd->input_fd != -1)
				close(cmd->input_fd);
			list = list->next;
			if ((cmd->input_fd = open(list->content, FLAG_I, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		list = list->next;
	}
}

void	redirection_output_fd(t_cmd *cmd, t_list *list)
{
	while (list)
	{
		if (!ft_strcmp(">", list->content))
		{ 
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd =
						open(list->content, FLAG_O, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		else if (!ft_strcmp(">>", list->content))
		{
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd =
						open(list->content, FLAG_AO, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		list = list->next;
	}
}

int		close_fds(t_cmd *cmd, pid_t pid, int pipe_open)
{
	int		status;
	int		ret;

	ret = EXIT_SUCCESS;
	status = 0;
	waitpid(pid, &status, 0);
	if (pipe_open)
	{
		close(cmd->pipes[PIPE_IN]);
		if (!cmd->next || cmd->type == TYPE_DEFAULT)
			close(cmd->pipes[PIPE_OUT]);
	}
	if (cmd->prev && cmd->prev->type == TYPE_PIPE)
		close(cmd->prev->pipes[PIPE_OUT]);
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
	if (cmd->output_fd != -1)
		close(cmd->output_fd);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	if (ret != EXIT_SUCCESS)
		ret = EXIT_FAILURE;
	return (ret);
}

void	child_process(t_msh *msh, t_cmd *cmd, t_exe_fn func)
{
	if (cmd->type == TYPE_PIPE &&
			dup2(cmd->pipes[PIPE_IN], STDOUT) < 0)
		exit_print_err(strerror(errno));
	if (cmd->prev && cmd->prev->type == TYPE_PIPE &&
			dup2(cmd->prev->pipes[PIPE_OUT], STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	func(msh, cmd);
	exit(EXIT_SUCCESS);
}

int		create_process(t_msh *msh, t_cmd *cmd, t_exe_fn func)
{
	pid_t	pid;
	int		ret;
	int		pipe_open;

	pipe_open = 0;
	ret = EXIT_SUCCESS;
	if (cmd->type == TYPE_PIPE)
	{
		pipe_open = 1;
		if (pipe(cmd->pipes) == -1)
			exit_print_err(strerror(errno));
	}
	if ((pid = fork()) == -1)
		exit_print_err(strerror(errno));
	if (pid == 0)
		child_process(msh, cmd, func);
	else
		ret = close_fds(cmd, pid, pipe_open);
	return (ret);
}


int	exec_process(t_msh *msh, t_cmd *cmd, char **av, char **env)
{
	char	*temp;
	int		idx;
	int		ret;

	idx = 0;
	while (msh->path[idx])
	{
		if (!(temp = ft_strjoin3(msh->path[idx], "/", cmd->args[0])))
			exit_print_err(strerror(errno));
		ret = execve(temp, av, env);
		free(temp);
		idx++;
	}
	if ((ret = execve(cmd->args[0], av, env)) && ret == -1)
		ret = command_not_found(msh->program_name, cmd->args[0]);
	ft_double_free((void **)av);
	ft_double_free((void **)env);
	return (ret);
}

int			executor(t_msh *msh, t_cmd *cmd)
{
	char	**av;
	char	**env;
	int		idx;

	if (!(av = (char **)malloc(sizeof(char *) * (cmd->length + 1))))
		exit_print_err(strerror(errno));
	if (!(av[0] = ft_strjoin("./", cmd->args[0])))
		exit_print_err(strerror(errno));
	idx = 1;
	while (idx < cmd->length)
	{
		if (!(av[idx] = ft_strdup(cmd->args[idx])))
			exit_print_err(strerror(errno));
		idx++;
	}
	av[idx] = NULL;
	env = ft_envjoin(msh->env, msh->env_len);
	return (exec_process(msh, cmd, av, env));
}


void	*compare_arg(t_msh *msh, t_cmd *cmd)
{
	if (!msh)
		return (NULL);
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (&do_exit);
	else if (!ft_strcmp(cmd->args[0], "echo"))
		return (&do_echo);
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (&do_env);
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (&do_export);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (&do_pwd);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (&do_unset);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (&do_cd);
	else
		return (&executor);
}

void	executing(t_msh *msh)
{
	t_exe_fn	func;
	t_cmd		*cmd;

	cmd = msh->cmds;
	while (cmd)
	{
		if (cmd->args[0] == NULL)
			break ;
		func = compare_arg(msh, cmd);
		redirection_input_fd(cmd, cmd->redirection_files);
		redirection_output_fd(cmd, cmd->redirection_files);
		if ((void *)func != &executor && cmd->type == TYPE_DEFAULT &&
			cmd->input_fd == -1 && cmd->output_fd == -1 &&
			(cmd->prev == NULL ||
			 (cmd->prev && cmd->prev->type == TYPE_DEFAULT)))
			func(msh, cmd);
		else
			create_process(msh, cmd, func);
		cmd = cmd->next;
	}
}
