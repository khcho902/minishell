/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 00:27:57 by kycho             #+#    #+#             */
/*   Updated: 2021/01/26 00:28:30 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			**malloc_env_array(t_dict **env)
{
	char	**array;
	int		not_null_env_cnt;
	int		i;

	not_null_env_cnt = 0;
	i = 0;
	while (env[i])
	{
		if (env[i]->value != NULL)
			not_null_env_cnt++;
		i++;
	}
	if (!(array = (char **)malloc(sizeof(char *) * (not_null_env_cnt + 2))))
		exit_print_err(strerror(errno));
	return (array);
}

char			**get_env_array(t_dict **env, char *exec_cmd)
{
	int		i;
	int		j;
	char	**array;

	array = malloc_env_array(env);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (env[i]->value != NULL)
		{
			if (!(array[j] = ft_strjoin3(env[i]->key, "=", env[i]->value)))
				exit_print_err(strerror(errno));
			j++;
		}
		i++;
	}
	if (!(array[j] = ft_strjoin("_=", exec_cmd)))
		exit_print_err(strerror(errno));
	array[j + 1] = NULL;
	return (array);
}
