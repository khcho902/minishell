/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 21:51:59 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/02 04:21:43 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_echo(t_msh *msh, int fd)
{
	char	*str;
	
	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
		ft_putchar_fd('\n', fd);
		return ;
	}
	str = msh->cmd_list[++msh->cmd_idx];
	ft_putstr_fd(str, fd);
	ft_putchar_fd('\n', fd);
}
