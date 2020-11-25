/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/25 22:49:44 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_pwd(int fd)
{
	char	buf[MAX_PATH];

	ft_putstr_fd(getcwd(buf, MAX_PATH), fd);
	ft_putchar_fd('\n', fd);
}

void		do_cd(t_msh *msh)
{
	char	*str;

	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
		chdir("/home/jiseo"); // HOME
		return ;
	}
	str = msh->cmd_list[++msh->cmd_idx];
	chdir(str);
}
