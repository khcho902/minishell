/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:00:35 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/02 04:35:20 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_env(t_list *env_list, int fd)
{
	t_list	*l;
	t_kv	*kv;

	l = env_list;
	while (l)
	{
		kv = l->content;
		ft_putstr_fd(kv->key, fd);
		ft_putchar_fd('=', fd);
		ft_putstr_fd(kv->value, fd);
		ft_putchar_fd('\n', fd);
		l = l->next;
	}
}
