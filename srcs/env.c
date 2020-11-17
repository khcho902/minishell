/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:00:35 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/17 19:01:56 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_kv		*key_value_generator(char *env)
{
	t_kv	*kv;
	char	*chr;
	int		i;

	i = 0;
	if (!(kv = (t_kv *)malloc(sizeof(t_kv))))
		return (NULL);
	chr = ft_strchr(env, '=');
	kv->key = ft_substr(env, 0, chr - env);
	kv->value = chr + 1;
	return (kv);
}

t_list		*init_env(char **env)
{
	t_list	*l;
	int		i;

	l = NULL;
	i = 0;
	while (env[i])
	{
		if (!l)
			l = ft_lstnew(key_value_generator(env[i]));
		else
			ft_lstadd_back(&l, ft_lstnew(key_value_generator(env[i])));
		i++;
	}
	return (l);
}

void		do_env(t_list *l)
{
	t_list	*l_cpy;
	t_kv	*kv;

	l_cpy = l;
	while (l_cpy)
	{
		kv = l_cpy->content;
		ft_putstr_fd(kv->key, STDIN_FILENO);
		ft_putchar_fd('=', STDIN_FILENO);
		ft_putstr_fd(kv->value, STDIN_FILENO);
		ft_putchar_fd('\n', STDIN_FILENO);
		l_cpy = l_cpy->next;
	}
}
