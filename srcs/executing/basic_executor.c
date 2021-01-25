/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 06:04:05 by kycho             #+#    #+#             */
/*   Updated: 2021/01/26 00:28:51 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			direct_basic_executor(t_msh *msh, t_cmd *cmd)
{
	char		**env;
	struct stat	sb;

	if (stat(cmd->args[0], &sb) == -1)
	{
		print_execute_err(msh->program_name, cmd->args[0], strerror(errno));
		exit(127);
	}
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
	{
		print_execute_err(msh->program_name, cmd->args[0], "is a directory");
		exit(126);
	}
	env = get_env_array(msh->env, cmd->args[0]);
	if ((execve(cmd->args[0], cmd->args, env)) == -1)
	{
		print_execute_err(msh->program_name, cmd->args[0], "Permission denied");
		ft_double_free((void*)env);
		exit(126);
	}
}

void			set_exec_cmd(t_cmd *cmd, char *path, char **exec_cmd)
{
	if (ft_strcmp(path, "") == 0)
	{
		free(*exec_cmd);
		if (!(*exec_cmd = ft_strdup(cmd->args[0])))
			exit_print_err(strerror(errno));
	}
	else
	{
		free(*exec_cmd);
		if (!(*exec_cmd = ft_strjoin3(path, "/", cmd->args[0])))
			exit_print_err(strerror(errno));
	}
}

void			execute_fail_handler(
			t_msh *msh, t_cmd *cmd, char *exec_cmd, int is_command_not_found)
{
	if (is_command_not_found == TRUE)
	{
		print_execute_err(msh->program_name, cmd->args[0], "command not found");
		exit(127);
	}
	else
	{
		print_execute_err(msh->program_name, exec_cmd, "Permission denied");
		exit(126);
	}
}

void			while_path_basic_executor(t_msh *msh, t_cmd *cmd, int i)
{
	struct stat	sb;
	char		**path;
	char		**env;
	int			is_command_not_found;
	char		*exec_cmd;

	path = split_path(msh->path);
	is_command_not_found = TRUE;
	exec_cmd = NULL;
	while (path[i])
	{
		set_exec_cmd(cmd, path[i], &exec_cmd);
		if (stat(exec_cmd, &sb) != -1 && (sb.st_mode & S_IFMT) != S_IFDIR)
		{
			is_command_not_found = FALSE;
			env = get_env_array(msh->env, exec_cmd);
			execve(exec_cmd, cmd->args, env);
			ft_double_free((void *)env);
		}
		i++;
	}
	execute_fail_handler(msh, cmd, exec_cmd, is_command_not_found);
}

void			basic_executor(t_msh *msh, t_cmd *cmd)
{
	if (cmd->args[0] == NULL)
		exit(0);
	if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	if ((ft_strcmp(msh->path, "") == 0) ||
			(ft_strchr(cmd->args[0], '/') != NULL))
		direct_basic_executor(msh, cmd);
	else
		while_path_basic_executor(msh, cmd, 0);
}
