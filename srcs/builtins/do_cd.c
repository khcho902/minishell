/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 11:59:13 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		do_cd(t_msh *msh)
{
	int		ret;
	char	*path;

	ret = EXIT_SUCCESS;
	if (msh->cmds->args[1] == NULL)
		path = get_env_dict(msh->env, "HOME")->value;
	else if (!ft_strcmp(msh->cmds->args[1], "-"))
		path = get_env_dict(msh->env, "OLDPWD")->value;
	else
		path = msh->cmds->args[1];
	if (chdir(path))
		ret = print_err("cd", path, "No such file or directory");
	return (ret);
}
