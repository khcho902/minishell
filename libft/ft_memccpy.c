/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 18:22:07 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/04 03:57:06 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	uc;

	i = 0;
	uc = c;
	while (i < n)
	{
		if ((((unsigned char *)dest)[i] = ((unsigned char *)src)[i]) == uc)
			return (dest + i + 1);
		i++;
	}
	return (NULL);
}
