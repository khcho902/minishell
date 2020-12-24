/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 00:09:18 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 00:31:43 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signo)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG);
	if (signo == SIGINT)
	{
		if (pid == -1)
		{
			ft_putchar_fd('\n', STDOUT);
			ft_putstr_fd(COLOR_BR_BLUE, STDOUT);
			ft_putstr_fd("minishell", STDOUT);
			ft_putstr_fd(COLOR_WHITE, STDOUT);
			ft_putstr_fd("$ ", STDOUT);
		}
		if (pid == 0)
			ft_putchar_fd('\n', STDOUT);
	}
	else if (signo == SIGQUIT)
	{
		if (pid == 0)
			ft_putstr_fd("Quit\n", STDOUT);
	}
}

void	init_signal(void)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
}
