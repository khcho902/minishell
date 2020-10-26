/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 01:11:18 by jiseo             #+#    #+#             */
/*   Updated: 2020/10/26 23:33:04 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#define BUFFER_SIZE 32

int				gnl_process(char **str, char **chr, char **temp, int fd)
{
	int				nread;
	char			buf[BUFFER_SIZE + 1];

	nread = 1;
	while (((*chr = ft_strchr(*str, '\n')) == NULL) && nread > 0)
	{
		if ((nread = read(fd, buf, BUFFER_SIZE)) == -1)
			return (-1);
		buf[nread] = '\0';
		if (*str == NULL)
			*temp = ft_strdup(buf);
		else
			*temp = ft_strjoin(*str, buf);
		free(*str);
		*str = *temp;
	}
	return (1);
}

int				get_next_line(int fd, char **line)
{
	static char		*str[1024];
	char			*chr;
	char			*temp;

	if (BUFFER_SIZE <= 0 || fd < 0 || fd > 1024 || line == NULL)
		return (-1);
	if (gnl_process(&str[fd], &chr, &temp, fd) == -1)
		return (-1);
	temp = str[fd];
	if (chr != NULL)
	{
		*line = ft_substr(str[fd], 0, chr - str[fd]);
		str[fd] = ft_strdup(chr + 1);
		free(temp);
		return (1);
	}
	else
	{
		*line = str[fd];
		str[fd] = NULL;
		return (0);
	}
}
