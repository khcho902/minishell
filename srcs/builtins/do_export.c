/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/02 04:22:44 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_export(t_msh *msh)
{
	char	*str;
	t_list	*l;
	t_kv	*kv;
	t_kv	*export_kv;

	l = msh->env_list;
	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
		printf("sort env\n");
		return ;
	}
	str = msh->cmd_list[++msh->cmd_idx];
	export_kv = key_value_generator(str);
	while (l)
	{
		kv = l->content;
		if (!ft_strncmp(export_kv->key, kv->key,
					ft_max(ft_strlen(export_kv->key), ft_strlen(kv->key))))
		{
			free(l->content);
			l->content = export_kv;
			return ;
		}
		l = l->next;
	}
	ft_lstadd_back(&msh->env_list, ft_lstnew(export_kv));
}
