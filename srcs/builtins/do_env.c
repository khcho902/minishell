/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:00:35 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/17 23:51:14 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_env(t_msh *msh, t_cmd *cmd)
{
	int i;

	(void)cmd;
	i = 0;
	while (msh->env[i])
	{
		if (msh->env[i]->value != NULL)
		{
			ft_putstr_fd(msh->env[i]->key, STDOUT);
			ft_putstr_fd("=", STDOUT);
			ft_putstr_fd(msh->env[i]->value, STDOUT);
			ft_putchar_fd('\n', STDOUT);
		}
		i++;
	}
	g_exit_status = 0;
}
