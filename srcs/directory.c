/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/21 01:36:54 by jiseo            ###   ########.fr       */
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

	str = msh->cmd_list[++msh->cmd_idx];
	chdir(str);
}
