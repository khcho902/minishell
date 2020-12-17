/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 12:26:15 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_cd(t_msh *msh)
{
	if (msh->cmds->args[1] == NULL)
		chdir("/home/jiseo"); // HOME
	else if (!ft_strcmp(msh->cmds->args[1], "-"))
		chdir("/home/jiseo/work"); // OLDPWD
	else
		chdir(msh->cmds->args[1]);
}
