/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:52 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/02 04:25:40 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_unset(t_msh *msh)
{
	char	*str;
	t_list	*l;
	t_list	*prev;
	t_list	*next;
	t_kv	*kv;

	str = msh->cmd_list[++msh->cmd_idx];
	l = msh->env_list;
	prev = NULL;
	while (l)
	{
		kv = l->content;
		next = l->next;
		if (!ft_strncmp(str, kv->key,
					ft_max(ft_strlen(str), ft_strlen(kv->key))))
		{
			if (kv)
				free(kv);
			free(l);
			if (prev)
				prev->next = next;
			else
				msh->env_list = next;
			return ;
		}
		prev = l;
		l = l->next;
	}
}
