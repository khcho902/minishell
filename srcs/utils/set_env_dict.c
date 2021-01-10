/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_dict.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 14:31:07 by kycho             #+#    #+#             */
/*   Updated: 2021/01/10 14:31:42 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_dict(t_msh *msh, char *key, char *value)
{
	t_dict	*dict;
	t_dict	**new_env;
	int		i;

	if (!(dict = (t_dict *)malloc(sizeof(t_dict) * 1)))
		exit_print_err(strerror(errno));
	if (!(dict->key = ft_strdup(key)))
		exit_print_err(strerror(errno));
	if (!(dict->value = ft_strdup(value)))
		exit_print_err(strerror(errno));
	if (!(new_env = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len + 2))))
		exit_print_err(strerror(errno));
	i = 0;
	while (i < msh->env_len)
	{
		new_env[i] = msh->env[i];
		i++;
	}
	new_env[msh->env_len] = dict;
	new_env[msh->env_len + 1] = NULL;
	msh->env_len++;
	free(msh->env);
	msh->env = new_env;
}

void	set_env_dict(t_msh *msh, char *key, char *value)
{
	t_dict *dict;

	dict = get_env_dict(msh->env, key);
	if (dict != NULL)
	{
		free(dict->value);
		if (!(dict->value = ft_strdup(value)))
			exit_print_err(strerror(errno));
	}
	else
		add_env_dict(msh, key, value);
}
