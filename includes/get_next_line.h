/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/24 22:02:28 by kycho             #+#    #+#             */
/*   Updated: 2021/01/23 23:06:06 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <unistd.h>
# include "../libft/libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef FD_NUMBER
#  define FD_NUMBER 256
# endif

typedef struct	s_gnl_material
{
	int			value_to_print_exist;
	char		buffer[BUFFER_SIZE];
	size_t		start_idx;
	size_t		last_idx;
}				t_gnl_material;

int				get_next_line(int fd, char **line);

#endif
