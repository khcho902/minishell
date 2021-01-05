/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 21:51:59 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/05 15:17:46 by kycho            ###   ########.fr       */
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
		if (cmd->length != (idx + 1))
			ft_putchar_fd(' ', STDOUT);
		idx++;
	}
	if (!n_option)
		ft_putchar_fd('\n', STDOUT);
	msh->exit_status = 0;
}
