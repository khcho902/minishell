/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 17:03:20 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/23 17:03:31 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dict		**copy_env(t_msh *msh, t_dict **dst)
{
	int		idx;

	idx = 0;
	while (idx < msh->env_len)
	{
		if (!(dst[idx] = (t_dict *)malloc(sizeof(t_dict))))
			exit_print_err(strerror(errno));
		if (!(dst[idx]->key = ft_strdup(msh->env[idx]->key)))
			exit_print_err(strerror(errno));
		if (!(dst[idx]->value = ft_strdup(msh->env[idx]->value)))
			exit_print_err(strerror(errno));
		idx++;
	}
	dst[idx] = NULL;
	return (dst);
}
