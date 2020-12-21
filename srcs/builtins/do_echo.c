/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 21:51:59 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/21 20:04:38 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_echo(t_msh *msh)
{
	int		idx;
	int		n_option;
	
	n_option = 0;
	if (msh->cmds->length > 1)
		if (!ft_strcmp(msh->cmds->args[1], "-n"))
			n_option = 1;
	idx = 1 + n_option;
	while (idx < msh->cmds->length)
	{
		ft_putstr_fd(msh->cmds->args[idx], STDOUT);
		ft_putchar_fd(' ', STDOUT);
		idx++;
	}
	if (!n_option)
		ft_putchar_fd('\n', STDOUT);
}
