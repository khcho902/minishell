/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 22:02:43 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/04 03:57:22 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	unsigned char		ch;
	size_t				i;

	ch = c;
	i = 0;
	while (i < n)
	{
		if (((unsigned char *)str)[i] == ch)
			return (((unsigned char *)str) + i);
		i++;
	}
	return (NULL);
}
