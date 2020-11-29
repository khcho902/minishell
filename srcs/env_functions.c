/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 21:51:59 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/29 21:46:42 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		do_echo(t_msh *msh, int fd)
{
	char	*str;
	
	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
		ft_putchar_fd('\n', fd);
		return ;
	}
	str = msh->cmd_list[++msh->cmd_idx];
	ft_putstr_fd(str, fd);
	ft_putchar_fd('\n', fd);
}

void		do_export(t_msh *msh)
{
	char	*str;
	t_list	*l;
	t_kv	*kv;
	t_kv	*export_kv;

	l = msh->env_list;
	if (msh->cmd_list[msh->cmd_idx + 1] == NULL)
	{
	//	print_export_env(l);
		printf("sorv env\n");
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
