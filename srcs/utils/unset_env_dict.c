/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env_dict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 15:14:50 by kycho             #+#    #+#             */
/*   Updated: 2021/01/12 15:15:23 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_dict(t_dict *dict)
{
	free(dict->key);
	free(dict->value);
	free(dict);
}

void	unset_env_dict(t_msh *msh, char *key)
{
	t_dict	*dict;
	t_dict	**new_env;
	int		i;
	int		new_i;

	dict = get_env_dict(msh->env, key);
	if (dict == NULL)
		return ;
	if (!(new_env = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len))))
		exit_print_err(strerror(errno));
	new_env[msh->env_len - 1] = NULL;
	i = 0;
	new_i = 0;
	while (i < msh->env_len)
	{
		if (ft_strcmp(msh->env[i]->key, key) == 0)
			free_dict(msh->env[i]);
		else
			new_env[new_i++] = msh->env[i];
		i++;
	}
	msh->env_len--;
	free(msh->env);
	msh->env = new_env;
}
