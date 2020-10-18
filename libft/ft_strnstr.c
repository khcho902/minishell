/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 07:04:24 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/04 04:07:15 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t size)
{
	size_t		i;
	size_t		to_find_len;

	if (*to_find == '\0')
		return ((char *)str);
	if (size == 0)
		return (NULL);
	i = 0;
	to_find_len = ft_strlen(to_find);
	while (str[i] != '\0' && i < size - to_find_len + 1)
	{
		if (str[i] == to_find[0] &&
			ft_strncmp(&str[i], to_find, to_find_len) == 0)
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}
