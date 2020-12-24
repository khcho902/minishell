/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:52 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 01:56:24 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_unset_len(t_msh *msh, char **args)
{
	int		idx;
	int		new_len;
	char	*chr;
	char	*key;

	idx = 1;
	new_len = msh->env_len;
	while (idx < msh->cmds->length)
	{
		if ((chr = ft_strchr(args[idx], '=')) != NULL)
		{
			if (!(key = ft_substr(args[idx], 0, chr - args[idx])))
				exit_print_err(strerror(errno));
			if (get_env_dict(msh->env, key) == NULL)
				new_len--;
			free(key);
		}
		idx++;
	}
	return (new_len);
}

int		check_args(char *key, char **args, int args_len)
{
	int		idx;

	idx = 1;
	while (idx < args_len)
	{
		if (!(ft_strcmp(key, args[idx])))
			return (0);
		idx++;
	}
	return (1);
}

void	do_unset(t_msh *msh)
{
	t_dict		**temp;
	int			idx;
	int			temp_idx;

	if (!(temp = (t_dict **)malloc(sizeof(t_dict *) *
					(get_unset_len(msh, msh->cmds->args) + 1))))
		exit_print_err(strerror(errno));
	idx = -1;
	temp_idx = 0;
	while (++idx < msh->env_len)
	{
		if (check_args(msh->env[idx]->key, msh->cmds->args, msh->cmds->length))
		{
			if (!(temp[temp_idx] = (t_dict *)malloc(sizeof(t_dict))))
				exit_print_err(strerror(errno));
			if (!(temp[temp_idx]->key = ft_strdup(msh->env[idx]->key)))
				exit_print_err(strerror(errno));
			if (!(temp[temp_idx]->value = ft_strdup(msh->env[idx]->value)))
				exit_print_err(strerror(errno));
			temp_idx++;
		}
	}
	temp[temp_idx] = NULL;
	env_free((void **)msh->env);
	msh->env_len = temp_idx;
	msh->env = temp;
}
