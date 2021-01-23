/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 06:04:05 by kycho             #+#    #+#             */
/*   Updated: 2021/01/21 06:04:34 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


	path_cnt = colon_cnt + 1;

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


	return (splited_path);

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
	char		**env;
	char		*temp;
	char		*temp2;
	int			idx;
	char		**path;
	int			is_command_not_found;
	struct stat	sb;

	if (cmd->args[0] == NULL)
		exit(0);
	if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	if ((ft_strcmp(msh->path, "") == 0) || (ft_strchr(cmd->args[0], '/') != NULL))
	{
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
			exit(126);
		}
	}
	else
	{
		path = split_path(msh->path);
		is_command_not_found = TRUE;
		temp2 = NULL;
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
			if (stat(temp, &sb) != -1 && (sb.st_mode & S_IFMT) != S_IFDIR)
			{
				if (temp2 == NULL)
				{
					if (!(temp2 = ft_strdup(temp)))
						exit_print_err(strerror(errno));
				}
				is_command_not_found = FALSE;
				env = get_env_array(msh->env, temp);
				execve(temp, cmd->args, env);
				ft_double_free((void *)env);
				//free(sb);
			}
		//	free(sb);
			free(temp);
			idx++;
		}
		if (is_command_not_found == TRUE)
		{
			print_execute_err(msh->program_name, cmd->args[0], "command not found");
			exit(127);
		}
		else
		{
		//	print_execute_err(msh->program_name, temp2, strerror(errno));
			print_execute_err(msh->program_name, temp2, "Permission denied");
			exit(126);
		}
		ft_double_free((void*)path); // 추가함  지워도 될듯?
	}
}
