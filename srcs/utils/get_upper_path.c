/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_upper_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 01:58:23 by kycho             #+#    #+#             */
/*   Updated: 2021/01/26 01:58:46 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_upper_path(char *path)
{
	char	*upper_path;
	int		i;
	int		slash_idx;

	slash_idx = 0;
	i = 0;
	while (path[i])
	{
		if (path[i] == '/')
			slash_idx = i;
		i++;
	}
	if (slash_idx == 0)
	{
		if (!(upper_path = ft_strdup("/")))
			exit_print_err(strerror(errno));
	}
	else
	{
		if (!(upper_path = malloc(sizeof(char) * (slash_idx + 1))))
			exit_print_err(strerror(errno));
		ft_strlcpy(upper_path, path, slash_idx + 1);
	}
	return (upper_path);
}
