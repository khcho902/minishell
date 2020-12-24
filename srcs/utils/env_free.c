/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 01:12:02 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 01:29:51 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_free(t_dict **env)
{
	int		idx;

	idx = 0;
	while (env[idx])
	{
		free(env[idx]->key);
		free(env[idx]->value);
		free(env[idx]);
		idx++;
	}
	if (env)
		free(env);
}
