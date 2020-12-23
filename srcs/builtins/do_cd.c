/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/22 11:29:18 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_cd(t_msh *msh)
{
	char	*path;
	char	*temp;
	t_dict	*pwd_dict;

	if (msh->cmds->args[1] == NULL)
		path = get_env_dict(msh->env, "HOME")->value;
	else if (!ft_strcmp(msh->cmds->args[1], "-"))
		path = get_env_dict(msh->env, "OLDPWD")->value;
	else
		path = msh->cmds->args[1];
	pwd_dict = NULL;
	if (chdir(path))
		exit_print_err(strerror(errno));
	else
	{
		if (!(temp = getcwd(NULL, 0)))
			exit_print_err(strerror(errno));
		pwd_dict = get_env_dict(msh->env, "PWD");
		free(pwd_dict->value);
		pwd_dict->value = temp;
	}
}
