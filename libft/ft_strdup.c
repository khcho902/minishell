/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 06:12:57 by jiseo             #+#    #+#             */
/*   Updated: 2020/08/09 02:01:15 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*dest;
	size_t	str_length;

	str_length = ft_strlen(str);
	if (!(dest = (char *)malloc(sizeof(char) * (str_length + 1))))
		return (NULL);
	ft_memcpy(dest, str, str_length);
	dest[str_length] = '\0';
	return (dest);
}
