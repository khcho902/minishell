/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 08:32:02 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 08:32:30 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_envjoin(t_dict **env, int env_len)
{
	char	**temp;
	int		idx;

	if (!(temp = (char **)malloc(sizeof(char *) * (env_len + 1))))
		exit_print_err(strerror(errno));
	idx = 0;
	while (idx < env_len)
	{
		if (!(temp[idx] = ft_strjoin3(env[idx]->key, "=", env[idx]->value)))
			exit_print_err(strerror(errno));
		idx++;
	}
	temp[idx] = NULL;
	return (temp);
}
