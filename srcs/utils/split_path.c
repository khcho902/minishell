/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 00:30:19 by kycho             #+#    #+#             */
/*   Updated: 2021/01/26 00:30:27 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				get_path_cnt(char *path_str)
{
	int colon_cnt;
	int i;

	colon_cnt = 0;
	i = 0;
	while (path_str[i])
	{
		if (path_str[i] == ':')
			colon_cnt++;
		i++;
	}
	return (colon_cnt + 1);
}

char			**split_path(char *path_str)
{
	int		i;
	int		path_idx;
	char	**splited_path;
	char	*tmp;

	if (!(splited_path = malloc(sizeof(char*) * (get_path_cnt(path_str) + 1))))
		exit_print_err(strerror(errno));
	if (!(tmp = ft_strdup("")))
		exit_print_err(strerror(errno));
	i = -1;
	path_idx = 0;
	while (path_str[++i])
	{
		if (path_str[i] == ':')
		{
			splited_path[path_idx++] = tmp;
			if (!(tmp = ft_strdup("")))
				exit_print_err(strerror(errno));
		}
		else
			append_char_to_str(&tmp, path_str[i]);
	}
	splited_path[path_idx++] = tmp;
	splited_path[path_idx] = NULL;
	return (splited_path);
}
