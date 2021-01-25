/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 06:04:05 by kycho             #+#    #+#             */
/*   Updated: 2021/01/26 00:18:30 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				get_path_cnt(char *path_str)
{
	int colon_cnt;
	int i;

	colon_cnt = 0;
	i = 0;
	while (path_str[i])
	{
		if (path_str[i] == ':')
			colon_cnt++;
		i++;
	}
	return (colon_cnt + 1);
}

char			**split_path(char *path_str)
{
	int		i;
	int		path_idx;
	char	**splited_path;
	char	*tmp;

	if (!(splited_path = malloc(sizeof(char*) * (get_path_cnt(path_str) + 1))))
		exit_print_err(strerror(errno));
	if (!(tmp = ft_strdup("")))
		exit_print_err(strerror(errno));
	i = -1;
	path_idx = 0;
	while (path_str[++i])
	{
		if (path_str[i] == ':')
		{
			splited_path[path_idx++] = tmp;
			if (!(tmp = ft_strdup("")))
				exit_print_err(strerror(errno));
		}
		else
			append_char_to_str(&tmp, path_str[i]);
	}
	splited_path[path_idx++] = tmp;
	splited_path[path_idx] = NULL;
	return (splited_path);
}

char			**malloc_env_array(t_dict **env)
{
	char	**array;
	int		not_null_env_cnt;
	int		i;

	not_null_env_cnt = 0;
	i = 0;
	while (env[i])
	{
		if (env[i]->value != NULL)
			not_null_env_cnt++;
		i++;
	}
	if (!(array = (char **)malloc(sizeof(char *) * (not_null_env_cnt + 2))))
		exit_print_err(strerror(errno));
	return (array);
}

char			**get_env_array(t_dict **env, char *exec_cmd)
{
	int		i;
	int		j;
	char	**array;

	array = malloc_env_array(env);
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
	if (!(array[j] = ft_strjoin("_=", exec_cmd)))
		exit_print_err(strerror(errno));
	array[j + 1] = NULL;
	return (array);
}

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
