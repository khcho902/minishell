/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:00:35 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 12:46:03 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_env(t_dict **env_arr, int fd)
{
	char	*temp;
	int		idx;

	idx = 0;
	while (env_arr[idx])
	{
		if (!(temp = ft_strjoin3(env_arr[idx]->key, "=", env_arr[idx]->value)))
			exit_print_err(strerror(errno));
		ft_putstr_fd(temp, fd);
		ft_putchar_fd('\n', fd);
		free(temp);
		idx++;
	}
}
