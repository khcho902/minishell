/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 01:11:18 by jiseo             #+#    #+#             */
/*   Updated: 2020/08/11 20:10:43 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#define BUFFER_SIZE 32

int				get_next_line(int fd, char **line)
{
	static char		*str[1024];
	char			buf[BUFFER_SIZE + 1];
	char			*chr;
	char			*temp;
	int				nread;

	nread = 1;
	if (BUFFER_SIZE <= 0 || fd < 0 || fd > 1024 || line == NULL)
		return (-1);
	while (((chr = ft_strchr(str[fd], '\n')) == NULL) && nread > 0)
	{
		if ((nread = read(fd, buf, BUFFER_SIZE)) == -1)
			return (-1);
		buf[nread] = '\0';
		temp = (str[fd] == NULL) ? ft_strdup(buf) : ft_strjoin(str[fd], buf);
		free(str[fd]);
		str[fd] = temp;
	}
	temp = str[fd];
	*line = (chr != NULL) ? ft_substr(str[fd], 0, chr - str[fd]) : str[fd];
	str[fd] = (chr != NULL) ? ft_strdup(chr + 1) : NULL;
	if (chr != NULL)
		free(temp);
	return (chr != NULL ? 1 : 0);
}
