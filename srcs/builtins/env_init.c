/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:35:43 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/02 04:35:54 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_kv	*key_value_generator(char *env)
{
	t_kv	*kv;
	char	*chr;

	if (!(kv = (t_kv *)malloc(sizeof(t_kv))))
		return (NULL);
	chr = ft_strchr(env, '=');
	kv->key = ft_substr(env, 0, chr - env);
	kv->value = chr + 1;
	return (kv);
}

t_list	*init_env(char **env)
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
