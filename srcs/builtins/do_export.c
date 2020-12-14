/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/15 03:50:30 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*sort_env(t_list *env, int size)
{
	t_list	*begin;
	t_list	*prev;
	t_list	*next;
	t_dict	*dict1;
	t_dict	*dict2;

	while (size-- && !(prev = NULL))
	{
		begin = env;
		while (env->next)
		{
			next = env->next;
			dict1 = env->content;
			dict2 = next->content;
			if (ft_strcmp(dict1->key, dict2->key) > 0)
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
	t_dict	*dict;
	t_dict	*export_dict;

	l = msh->env_list;
	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
		printf("--- export start ---[size:%d]\n", ft_lstsize(l));
		print_env(sort_env(l, ft_lstsize(l)), STDOUT);
		printf("--- export end ---\n");
		return ;
	}
	str = msh->cmd_list[++msh->cmd_idx];
	export_dict = dict_generator(str);
	while (l)
	{
		dict = l->content;
		if (!ft_strcmp(export_dict->key, dict->key))
		{
			free(l->content);
			l->content = export_dict;
			return ;
		}
		l = l->next;
	}
	ft_lstadd_back(&msh->env_list, ft_lstnew(export_dict));
}
