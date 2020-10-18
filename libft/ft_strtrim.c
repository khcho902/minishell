/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 19:33:05 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/04 04:07:33 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char		*ft_strtrim(char const *str, char const *set)
{
	int		start;
	int		end;

	if (!str || !set)
		return (NULL);
	start = -1;
	while (str[++start] != '\0')
		if (is_in_set(str[start], set) == 0)
			break ;
	end = ft_strlen(str);
	if (start >= end)
		return (ft_strdup(""));
	while (end-- > 0)
		if (is_in_set(str[end], set) == 0)
			break ;
	return (ft_substr(str, start, end - start + 1));
}
