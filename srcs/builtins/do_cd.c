/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/26 01:59:02 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	normal_set_cd_path_sub(
						char **splited_path, char **path, int i, char *tmp)
{
	while (splited_path[i])
	{
		if (ft_strcmp(splited_path[i], "..") == 0)
		{
			tmp = get_upper_path(*path);
			free(*path);
			*path = tmp;
		}
		else if (ft_strcmp(splited_path[i], ".") != 0)
		{
			if (ft_strcmp(*path, "/") == 0)
			{
				if (!(tmp = ft_strjoin(*path, splited_path[i])))
					exit_print_err(strerror(errno));
			}
			else
			{
				if (!(tmp = ft_strjoin3(*path, "/", splited_path[i])))
					exit_print_err(strerror(errno));
			}
			free(*path);
			*path = tmp;
		}
		i++;
	}
}

int		normal_set_cd_path(t_msh *msh, t_cmd *cmd, char **path)
{
	char *tmp;
	char **splited_path;

	if (cmd->args[1][0] == '/')
	{
		if (!(tmp = ft_strdup(cmd->args[1])))
			exit_print_err(strerror(errno));
	}
	else
	{
		if (!(tmp = ft_strjoin3(msh->pwd, "/", cmd->args[1])))
			exit_print_err(strerror(errno));
	}
	if (!(splited_path = ft_split(tmp, '/')))
		exit_print_err(strerror(errno));
	free(tmp);
	if (!(*path = ft_strdup("/")))
		exit_print_err(strerror(errno));
	normal_set_cd_path_sub(splited_path, path, 0, NULL);
	ft_double_free((void *)splited_path);
	return (SUCCESS);
}

int		set_cd_path(t_msh *msh, t_cmd *cmd, char **path, char *p_name)
{
	t_dict *dict;

	if (cmd->length == 1)
	{
		if (!(dict = get_env_dict(msh->env, "HOME")) || !(dict->value))
			return (print_execute_err(p_name, cmd->args[0], "HOME not set"));
		if (!(*path = ft_strdup(dict->value)))
			exit_print_err(strerror(errno));
		return (SUCCESS);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if (!(dict = get_env_dict(msh->env, "OLDPWD")) || !(dict->value))
			return (print_execute_err(p_name, cmd->args[0], "OLDPWD not set"));
		if (!(*path = ft_strdup(dict->value)))
			exit_print_err(strerror(errno));
		return (SUCCESS);
	}
	else if (!ft_strcmp(cmd->args[1], "") || !ft_strcmp(cmd->args[1], "."))
	{
		if (!(*path = ft_strdup(msh->pwd)))
			exit_print_err(strerror(errno));
		return (SUCCESS);
	}
	return (normal_set_cd_path(msh, cmd, path));
}

void	set_oldpwd_pwd(t_msh *msh, char *path)
{
	if (get_env_dict(msh->env, "OLDPWD"))
	{
		if (get_env_dict(msh->env, "PWD"))
			set_env_dict(msh, "OLDPWD", get_env_dict(msh->env, "PWD")->value);
		else if (msh->unset_pwd_flag == 1)
		{
			set_env_dict(msh, "OLDPWD", "");
			msh->unset_pwd_flag = 0;
		}
		else
			set_env_dict(msh, "OLDPWD", msh->pwd);
	}
	if (get_env_dict(msh->env, "PWD"))
		set_env_dict(msh, "PWD", path);
	free(msh->pwd);
	msh->pwd = path;
}

void	do_cd(t_msh *msh, t_cmd *cmd)
{
	char	*path;
	char	*tmp;

	if (set_cd_path(msh, cmd, &path, msh->program_name) == ERROR)
		return ;
	if (chdir(path))
	{
		if (!(tmp = ft_strjoin3(cmd->args[0], ": ", cmd->args[1])))
			exit_print_err(strerror(errno));
		print_execute_err(msh->program_name, tmp, strerror(errno));
		free(tmp);
		free(path);
		g_exit_status = 1;
		return ;
	}
	set_oldpwd_pwd(msh, path);
	g_exit_status = 0;
}
