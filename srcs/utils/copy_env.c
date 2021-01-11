/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 17:03:20 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/24 15:19:53 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_env(t_msh *msh, t_dict **dst)
{
	int		idx;

	idx = 0;
	while (idx < msh->env_len)
	{
		
		
		if (!(dst[idx] = (t_dict *)malloc(sizeof(t_dict))))
			exit_print_err(strerror(errno));
		if (!(dst[idx]->key = ft_strdup(msh->env[idx]->key)))
			exit_print_err(strerror(errno));
		if (msh->env[idx]->value == NULL)
		{
			dst[idx]->value = NULL;
		}
		else
		{
			if (!(dst[idx]->value = ft_strdup(msh->env[idx]->value)))
				exit_print_err(strerror(errno));
		}
		
		idx++;
	}
	dst[idx] = NULL;
}
