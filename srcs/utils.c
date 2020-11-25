/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 16:32:07 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/25 19:14:14 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_double_free(char **ptr)
{
	int		i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	if (ptr)
		free(ptr);
}

int			ft_max(int n1, int n2)
{
	if (n1 > n2)
		return (n1);
	else
		return (n2);
}
/*
static void	sprite_swap(t_sprite *a, t_sprite *b)
{
	t_sprite	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void		quick_sort_sprite(int left, int right, t_info *info)
{
	int		pivot;
	int		i;
	int		j;

	pivot = left;
	i = left + 1;
	j = pivot;
	if (!(left < right))
		return ;
	while (i <= right)
	{
		if (info->sprite[i].dist > info->sprite[pivot].dist)
		{
			j++;
			sprite_swap(&info->sprite[j], &info->sprite[i]);
		}
		i++;
	}
	sprite_swap(&info->sprite[left], &info->sprite[j]);
	pivot = j;
	quick_sort_sprite(left, pivot - 1, info);
	quick_sort_sprite(pivot + 1, right, info);
}*/
