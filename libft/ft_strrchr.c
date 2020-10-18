/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 06:50:52 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/04 04:07:24 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	size_t		str_length;
	size_t		i;
	char		ch;

	str_length = ft_strlen(str);
	i = 0;
	if ((unsigned char)c == 0)
		return ((char *)&str[str_length]);
	ch = c;
	while (str_length - i > 0)
	{
		if (str[str_length - i - 1] == ch)
			return ((char *)&str[str_length - i - 1]);
		i++;
	}
	return (NULL);
}
