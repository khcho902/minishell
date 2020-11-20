/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 16:32:07 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/21 00:48:31 by jiseo            ###   ########.fr       */
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

int		exact_strcmp(char *str1, char *str2)
{
	int		str1_len;
	int		str2_len;

	str1_len = ft_strlen(str1);
	str2_len = ft_strlen(str2);
	if ((str1_len == str2_len) && ft_strncmp(str1, str2, str1_len) == 0)
		return (1);
	return (0);
}
