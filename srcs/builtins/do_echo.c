/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 21:51:59 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 15:45:34 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_echo(t_msh *msh, t_cmd *cmd)
{
	int		idx;
	int		n_option;
	
	n_option = 0;
	if (cmd->length > 1)
		if (!ft_strcmp(cmd->args[1], "-n"))
			n_option = 1;
	idx = 1 + n_option;
	while (idx < cmd->length)
	{
		ft_putstr_fd(cmd->args[idx], STDOUT);
		ft_putchar_fd(' ', STDOUT);
		idx++;
	}
	if (!n_option)
		ft_putchar_fd('\n', STDOUT);
	(void)msh;
}
