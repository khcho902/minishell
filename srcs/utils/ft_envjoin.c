/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 08:32:02 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/19 00:54:25 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_envjoin(t_dict **env, int env_len)
{
	char	**temp;
	int		idx;
	int		i;


	dprintf(2, "--------------\n");
	i = 0;
	while (env[i])
	{
		dprintf(2, "key : %s\n", env[i]->key);
		if (env[i]->value == NULL)
		{
			dprintf(2, "vakue : NULL\n");
		}
		else
		{
			dprintf(2, "value : %s\n", env[i]->value);
		}
		i++;
	}
	dprintf(2, "--------------\n");

	if (!(temp = (char **)malloc(sizeof(char *) * (env_len + 1))))
		exit_print_err(strerror(errno));
	i = 0;
	idx = 0;
	while (i < env_len)
	{
		if (env[i]->value != NULL)
		{
			if (env[idx]->value == NULL)
				temp[idx] = ft_strdup(env[idx]->key);
			else
				temp[idx] = ft_strjoin3(env[idx]->key, "=", env[idx]->value);
				
			if (temp[idx] == NULL)
				exit_print_err(strerror(errno));
			idx++;	
		}
		i++;
	}
	while (idx <= env_len)
	{
		temp[idx] = NULL;
		idx++;
	}
	return (temp);
}
