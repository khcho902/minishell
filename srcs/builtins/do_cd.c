/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/21 20:03:56 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_cd(t_msh *msh)
{
	char	*path;

	if (msh->cmds->args[1] == NULL)
		path = get_env_dict(msh->env, "HOME")->value;
	else if (!ft_strcmp(msh->cmds->args[1], "-"))
		path = get_env_dict(msh->env, "OLDPWD")->value;
	else
		path = msh->cmds->args[1];
	if (chdir(path))
		exit_print_err(strerror(errno));
}
