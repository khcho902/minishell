/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:52 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 12:53:40 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_args(char **args, char *key)
{
	int		idx;

	idx = 0;
	while (args[idx])
	{
		if (!ft_strcmp(args[idx], key))
			return (idx);
		idx++;
	}
	return (0);
}

void		do_unset(t_msh *msh)
{
	t_dict		**temp;
	int			idx;
	int			env_idx;

	if (!(temp = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len))))
		exit_print_err(strerror(errno));
	idx = 0;
	while (idx < msh->env_len - (msh->cmds->length - 1))
		if (!(temp[idx++] = (t_dict *)malloc(sizeof(t_dict))))
			exit_print_err(strerror(errno));
	idx = 0;
	env_idx = 0;
	while (msh->env[env_idx])
	{
		if (!check_args(&msh->cmds->args[1], msh->env[env_idx]->key))
		{
			if (!(temp[idx]->key = ft_strdup(msh->env[env_idx]->key)))
				exit_print_err(strerror(errno));
			if (!(temp[idx]->value = ft_strdup(msh->env[env_idx]->value)))
				exit_print_err(strerror(errno));
			idx++;
		}
		env_idx++;
	}
	temp[idx] = NULL;
	ft_double_free((void **)msh->env);
	msh->env_len -= (msh->cmds->length - 1);
	msh->env = temp;
}
