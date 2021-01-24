/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 21:51:59 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/06 20:07:46 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_n_option(char *token)
{
	int	i;

	if (ft_strncmp(token, "-n", 2) != 0)
		return (FALSE);
	i = 2;
	while (token[i])
	{
		if (token[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	do_echo(t_msh *msh, t_cmd *cmd)
{
	int		idx;
	int		n_option;

	(void)msh;
	n_option = 0;
	idx = 1;
	while (check_n_option(cmd->args[idx]))
	{
		n_option = 1;
		idx++;
	}
	while (cmd->args[idx])
	{
		ft_putstr_fd(cmd->args[idx], STDOUT);
		if (cmd->length != (idx + 1))
			ft_putchar_fd(' ', STDOUT);
		idx++;
	}
	if (!n_option)
		ft_putchar_fd('\n', STDOUT);
	g_exit_status = 0;
}
