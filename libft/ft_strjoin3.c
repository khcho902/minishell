/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 03:25:46 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/10 03:25:55 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin3(char *str1, char *str2, char *str3)
{
	char	*temp;
	int		i;
	int		sum_len;

	sum_len = ft_strlen(str1) + ft_strlen(str2) + ft_strlen(str3);
	temp = (char *)malloc(sizeof(char) * (sum_len + 1));
	if (!temp)
		return (NULL);
	i = 0;
	while (*str1)
		temp[i++] = *str1++;
	while (*str2)
		temp[i++] = *str2++;
	while (*str3)
		temp[i++] = *str3++;
	temp[i] = '\0';
	return (temp);
}
