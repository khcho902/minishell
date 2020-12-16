/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 16:32:07 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 06:06:23 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_double_free(char **ptr)
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

int		is_in_charset(char c, char *str)
{
	int idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == c)
			return (TRUE);
		idx++;
	}
	return (FALSE);
}
