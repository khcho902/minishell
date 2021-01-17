/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/17 23:52:49 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_upper_path(char *path)
{
	char	*upper_path;
	int		i;
	int		slash_idx;
	
	slash_idx = 0;
	i = 0;
	while (path[i])
	{
		if (path[i] == '/')
			slash_idx = i;
		i++;
	}
	if (slash_idx == 0)
	{
		if (!(upper_path = ft_strdup("/")))
			exit_print_err(strerror(errno));
	}
	else
	{
		if (!(upper_path = malloc(sizeof(char) * (slash_idx + 1))))
			exit_print_err(strerror(errno));
		ft_strlcpy(upper_path, path, slash_idx + 1);
	}
	return (upper_path);
}

int		set_cd_path(t_msh *msh, t_cmd *cmd, char **path)
{
	char *p_name;
	t_dict *env_dict;

	p_name = msh->program_name;
	if (cmd->length == 1)
	{
		if (!(env_dict = get_env_dict(msh->env, "HOME")))
			return (print_execute_err(p_name, cmd->args[0], "HOME not set"));
		if (!(*path = ft_strdup(env_dict->value)))
			exit_print_err(strerror(errno));
		return (SUCCESS);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if (!(env_dict = get_env_dict(msh->env, "OLDPWD")))
			return (print_execute_err(p_name, cmd->args[0], "OLDPWD not set"));
		if (!(*path = ft_strdup(env_dict->value)))
			exit_print_err(strerror(errno));
		return (SUCCESS);
	}
	else if ((ft_strcmp(cmd->args[1], "") == 0) || (ft_strcmp(cmd->args[1], ".") == 0))
	{
		if (!(*path = ft_strdup(msh->pwd)))
			exit_print_err(strerror(errno));
		return (SUCCESS);
	}
	
	
	char *tmp;
	if (cmd->args[1][0] == '/')
		tmp = ft_strdup(cmd->args[1]);
	else
		tmp = ft_strjoin3(msh->pwd, "/", cmd->args[1]);
	char **splited_path = ft_split(tmp, '/');
	free(tmp);
	*path = ft_strdup("/");
	int i = 0;
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
				tmp = ft_strjoin(*path, splited_path[i]);
			else
				tmp = ft_strjoin3(*path, "/", splited_path[i]);
			free(*path);
			*path = tmp;
		}
		i++;
	}
	ft_double_free((void *)splited_path);
	return (SUCCESS);
}

void	do_cd(t_msh *msh, t_cmd *cmd)
{
	char	*path;
	char	*tmp;
	
	if (set_cd_path(msh, cmd, &path) == ERROR)
		return ;
	if (chdir(path))
	{
		if (!(tmp = ft_strjoin3(cmd->args[0], ": ", cmd->args[1])))
			exit_print_err(strerror(errno));
		print_execute_err(msh->program_name, tmp, strerror(errno));
		free(tmp);
		free(path);
		msh->exit_status = 1;
		return ;
	}
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
	msh->exit_status = 0;
}
