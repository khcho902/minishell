/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/05 05:03:22 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*sort_env(t_list *env, int size)
{
	t_list	*begin;
	t_list	*prev;
	t_list	*next;
	t_kv	*kv1;
	t_kv	*kv2;

	while (size-- && !(prev = NULL))
	{
		begin = env;
		while (env->next)
		{
			next = env->next;
			kv1 = env->content;
			kv2 = next->content;
			if (ft_strncmp(kv1->key, kv2->key,
						ft_max(ft_strlen(kv1->key), ft_strlen(kv2->key))) > 0)
			{
				env->next = next->next;
				next->next = env;
				env = next;
				if (!prev)
					begin = env;
				else
					prev->next = env;
			}
			prev = env;
			env = env->next;
		}
		env = begin;
	}
	return (env);
}

void			do_export(t_msh *msh)
{
	char	*str;
	t_list	*l;
	t_kv	*kv;
	t_kv	*export_kv;

	l = msh->env_list;
	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
		printf("--- export start ---[size:%d]\n", ft_lstsize(l));
		print_env(sort_env(l, ft_lstsize(l)), STDOUT);
		printf("--- export end ---\n");
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
