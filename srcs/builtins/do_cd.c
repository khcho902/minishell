/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 00:57:53 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/02 00:58:04 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_cd(t_msh *msh)
{
	char	*str;

	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
		chdir("/home/jiseo"); // HOME
		return ;
	}
	str = msh->cmd_list[++msh->cmd_idx];
	if (!ft_strncmp("-", str, ft_max(1, ft_strlen(str))))
	{
		chdir("/home/jiseo/work"); // OLDPWD
		return ;
	}
	chdir(str);
	/*
		str = ft_strdup();
		i = chdir(str);
		if (str == 0)
			success;
		else if (str == -1)
			fail;
	*/ 
}
