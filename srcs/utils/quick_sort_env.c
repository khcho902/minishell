/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 09:13:17 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/21 03:40:35 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		dict_swap(t_dict **a, t_dict **b)
{
	t_dict		*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void			quick_sort_env(int left, int right, t_dict **env)
{
	int			pivot;
	int			i;
	int			j;

	pivot = left;
	i = left + 1;
	j = pivot;
	if (!(left < right))
		return ;
	while (i <= right)
	{
		if (ft_strcmp(env[i]->key, env[pivot]->key) < 0)
		{
			j++;
			dict_swap(&env[j], &env[i]);
		}
		i++;
	}
	dict_swap(&env[left], &env[j]);
	pivot = j;
	quick_sort_env(left, pivot - 1, env);
	quick_sort_env(pivot + 1, right, env);
}
