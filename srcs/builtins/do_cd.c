/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/11 23:26:44 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		set_cd_path(t_msh *msh, t_cmd *cmd, char **path)
{
	char *p_name;
	t_dict *env_dict;

	p_name = msh->program_name;
	if (cmd->length == 1)
	{
		if (!(env_dict = get_env_dict(msh->env, "HOME")))
			return (print_execute_err(p_name, cmd->args[0], "HOME not set"));
		*path = env_dict->value;
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if (ft_strcmp(msh->oldpwd, "") == 0)
			return (print_execute_err(p_name, cmd->args[0], "OLDPWD not set"));
		*path = msh->oldpwd;
	}
	else if (ft_strcmp(cmd->args[1], "") == 0)
		*path = ".";	
	else
		*path = cmd->args[1];
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
		if (!(tmp = ft_strjoin3(cmd->args[0], ": ", path)))
			print_execute_err(msh->program_name, tmp, strerror(errno));
		print_execute_err(msh->program_name, tmp, strerror(errno));
		free(tmp);
		return ;
	}
	free(msh->oldpwd);
	if (!(msh->oldpwd = ft_strdup(msh->pwd)))
		exit_print_err(strerror(errno));
	free(msh->pwd);
	if (!(msh->pwd = getcwd(NULL, 0)))
		exit_print_err(strerror(errno));
	if (get_env_dict(msh->env, "OLDPWD"))
		set_env_dict(msh, "OLDPWD", msh->oldpwd);
	if (get_env_dict(msh->env, "PWD"))
		set_env_dict(msh, "PWD", msh->pwd);
}
