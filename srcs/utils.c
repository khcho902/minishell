/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 16:32:07 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/29 07:40:42 by jiseo            ###   ########.fr       */
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
