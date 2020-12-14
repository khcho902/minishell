/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:52 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/15 02:52:40 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_unset(t_msh *msh)
{
	char	*str;
	t_list	*l;
	t_list	*prev;
	t_list	*next;
	t_dict	*dict;

	str = msh->cmd_list[++msh->cmd_idx];
	l = msh->env_list;
	prev = NULL;
	while (l)
	{
		dict = l->content;
		next = l->next;
		if (!ft_strncmp(str, dict->key,
					ft_max(ft_strlen(str), ft_strlen(dict->key))))
		{
			if (dict)
				free(dict);
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
