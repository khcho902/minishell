/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 09:52:38 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/10 04:00:22 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char	*result;
	size_t	i;
	size_t	length;

	if (!s || !f)
		return (NULL);
	length = ft_strlen(s);
	if (!(result = (char *)malloc(sizeof(char) * (length + 1))))
		return (NULL);
	i = 0;
	while (i < length)
	{
		result[i] = (*f)(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
