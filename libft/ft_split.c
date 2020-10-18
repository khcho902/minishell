/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 01:40:26 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/12 09:47:21 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		get_r_idx(char const *s, char c)
{
	int		count;
	int		is_not_c;

	count = 0;
	is_not_c = 0;
	while (*s)
	{
		if (*s == c)
		{
			if (is_not_c != 0)
				count++;
			is_not_c = 0;
		}
		else
			is_not_c = 1;
		s++;
	}
	return (count + is_not_c + 1);
}

static void		*split_process(char const *s, char c, char **rst, int len)
{
	int		copy_len;
	int		i;

	copy_len = 0;
	i = 0;
	while (len-- >= 0)
	{
		if (*s == c || *s == '\0')
		{
			if (copy_len != 0)
			{
				if (!(rst[i] = (char *)malloc(sizeof(char) * (copy_len + 1))))
					return (NULL);
				ft_memcpy(rst[i], s - copy_len, copy_len);
				rst[i++][copy_len] = '\0';
			}
			copy_len = 0;
		}
		else
			copy_len++;
		s++;
	}
	rst[i] = NULL;
	return (rst);
}

char			**ft_split(char const *s, char c)
{
	char	**rst;

	if (!s || !(rst = (char **)malloc(sizeof(char *) * get_r_idx(s, c))))
		return (NULL);
	return (split_process(s, c, rst, ft_strlen(s)));
}
