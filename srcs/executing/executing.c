/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:03:05 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/19 15:33:52 by kycho            ###   ########.fr       */
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


char			**split_path(char *path_str)
{
	int		i;
	int		path_idx;
	int		colon_cnt;
	char	**splited_path;
	int		path_cnt;
	char	*tmp;

	colon_cnt = 0;
	i = 0;
	while (path_str[i])
	{
		if (path_str[i] == ':')
			colon_cnt++;
		i++;
	}

//	dprintf(2, "colon_cnt : %d\n", colon_cnt);


	path_cnt = colon_cnt + 1;

//	dprintf(2, "path_cnt : %d\n", path_cnt);



	if (!(splited_path = (char **)malloc(sizeof(char*) * (path_cnt + 1))))
		exit_print_err(strerror(errno));

	
	
	if (!(tmp = ft_strdup("")))
		exit_print_err(strerror(errno));

	i = 0;
	path_idx = 0;
	while (path_str[i])
	{
		if (path_str[i] == ':')
		{
			splited_path[path_idx] = tmp;
			if (!(tmp = ft_strdup("")))
				exit_print_err(strerror(errno));	
			path_idx++;
		}
		else
		{
			append_char_to_str(&tmp, path_str[i]);	
		}
		i++;
	}
	splited_path[path_idx] = tmp;
	path_idx++;

	splited_path[path_idx] = NULL;


/*
	if (path_cnt != path_idx)
	{
		dprintf(2, "patn_cnt = %d\n", path_cnt);
		dprintf(2, "path_idx = %d\n", path_idx);
		dprintf(2, "nonononnonononnononononononnoonononononn\n");
	}
*/
	return (splited_path);

}

int				is_directory(char *path)
{
	struct stat sb;

	if (stat(path, &sb) == -1)
		return (FALSE);
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
}

char			**get_env_array(t_dict **env, char *command)
{
	int i;
	int j;
	char **array;

	j = 0;
	i = 0;
	while (env[i])
	{
		if (env[i]->value != NULL)
			j++;
		i++;
	}
	if (!(array = (char **)malloc(sizeof(char *) * (j + 2))))
		exit_print_err(strerror(errno));
	i = 0;
	j = 0;
	while (env[i])
	{
		if (env[i]->value != NULL)
		{
			if (!(array[j] = ft_strjoin3(env[i]->key, "=", env[i]->value)))
				exit_print_err(strerror(errno));
			j++;
		}
		i++;
	}
	if (!(array[j] = ft_strjoin("_=", command)))
		exit_print_err(strerror(errno));
	array[j + 1] = NULL;
	return (array);
}

void			basic_executor(t_msh *msh, t_cmd *cmd)
{
	char	**env;
	char	*temp;
	int		idx;
	char	**path;   //추가 

	if (cmd->args[0] == NULL)
		exit(0);


	if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	
//	env = ft_envjoin(msh->env, msh->env_len);
	
	if ((ft_strcmp(msh->path, "") == 0) || (ft_strchr(cmd->args[0], '/') != NULL))
	{
		if (is_directory(cmd->args[0]) == TRUE)
		{
			print_execute_err(msh->program_name, cmd->args[0], "is a directory");
			exit(126);
		}
		env = get_env_array(msh->env, cmd->args[0]);
		if ((execve(cmd->args[0], cmd->args, env)) == -1)
		{
			print_execute_err(msh->program_name, cmd->args[0], strerror(errno));
			if (errno == 13)
				exit(126);
			exit(127);
		}
	}
	/*
	else if (ft_strchr(msh->path, ':') == NULL && ft_strchr(msh->path, '/') == NULL)   // 좀더 생각해봐야할듯...
	{
		env = get_env_array(msh->env, cmd->args[0]);
		if (!(temp = ft_strjoin3(msh->path, "/", cmd->args[0])))
			exit_print_err(strerror(errno));
		if ((execve(temp, cmd->args, env)) == -1)
		{
			print_execute_err(msh->program_name, temp, strerror(errno));
			if (errno == 13)
				exit(126);
			exit(127);
		}	
	}
	*/
	else
	{
		//dprintf(2, "in else\n");

		path = split_path(msh->path); // 추가 
		/*
			if (!(path = ft_split(msh->path, ':'))) // 추가 
				exit_print_err(strerror(errno));
		*/
		/*
			int i = 0;
			while (path[i])
			{
				dprintf(2 , "|%s|\n", path[i]);
				i++;
			}
		*/
		idx = 0;
		while (path[idx])
		{
			if (ft_strcmp(path[idx], "") == 0)
			{
				if (!(temp = ft_strdup(cmd->args[0])))
					exit_print_err(strerror(errno));
			}
			else
			{
				if (!(temp = ft_strjoin3(path[idx], "/", cmd->args[0])))
					exit_print_err(strerror(errno));	
			}
			env = get_env_array(msh->env, temp);
			execve(temp, cmd->args, env);
			ft_double_free((void *)env);
			free(temp);
			idx++;
		}
		print_execute_err(msh->program_name, cmd->args[0], "command not found");
		exit(127);
		ft_double_free((void*)path); // 추가함  지워도 될듯?
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
		free(msh->lstcmd);
		if (cmd->length == 0)
		{
			if (!(msh->lstcmd = ft_strdup("")))
				exit_print_err(strerror(errno));
		}
		else
		{
			if (!(msh->lstcmd = ft_strdup(cmd->args[cmd->length - 1])))
				exit_print_err(strerror(errno));
		}

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

		unset_env_dict(msh, "_");
	}
}
