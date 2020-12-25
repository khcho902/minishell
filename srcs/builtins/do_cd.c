/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 15:33:07 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_env_values(t_msh *msh)
{
	t_dict	*oldpwd_dict;
	t_dict	*pwd_dict;

	oldpwd_dict = NULL;
	pwd_dict = NULL;
	if ((pwd_dict = get_env_dict(msh->env, "PWD")) != NULL)
	{
		if ((oldpwd_dict = get_env_dict(msh->env, "OLDPWD")) != NULL)
			free_and_get_value(&(oldpwd_dict->value), pwd_dict->value);
	}
	else
		free(oldpwd_dict->value);
	if (pwd_dict != NULL)
	{
		free(pwd_dict->value);
		if (!(pwd_dict->value = getcwd(NULL, 0)))
			exit_print_err(strerror(errno));
	}
}

void	do_cd(t_msh *msh, t_cmd *cmd)
{
	char	*path;

	if (cmd->args[1] == NULL)
		path = get_env_dict(msh->env, "HOME")->value;
	else if (!ft_strcmp(cmd->args[1], "-"))
		path = get_env_dict(msh->env, "OLDPWD")->value;
	else
		path = cmd->args[1];
	if (chdir(path))
		nonexit_print_err(strerror(errno));
	set_env_values(msh);
}
