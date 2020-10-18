/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 08:42:11 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/04 04:06:28 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	s1_length;
	size_t	s2_length;
	size_t	alloc_len;

	if (!s1 || !s2)
		return (NULL);
	s1_length = ft_strlen(s1);
	s2_length = ft_strlen(s2);
	alloc_len = s1_length + s2_length + 1;
	if ((dest = (char *)ft_calloc(alloc_len, sizeof(char))) == NULL)
		return (NULL);
	dest = ft_memmove(dest, s1, s1_length);
	ft_strlcat(dest, s2, alloc_len);
	return (dest);
}
