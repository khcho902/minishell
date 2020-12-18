/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 10:41:44 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_dict	**copy_env(t_msh *msh, t_dict **dest)
{
	int			idx;

	idx = 0;
	while (idx < msh->env_len)
	{
		if (!(dest[idx] = (t_dict *)malloc(sizeof(t_dict))))
			exit_print_err(strerror(errno));
		if (!(dest[idx]->key = ft_strdup(msh->env[idx]->key)))
			exit_print_err(strerror(errno));
		if (!(dest[idx]->value = ft_strdup(msh->env[idx]->value)))
			exit_print_err(strerror(errno));
		idx++;
	}
	dest[idx] = NULL;
	return (dest);
}

static t_dict	**export_env(t_msh *msh, t_dict **dest)
{
	char		*arg;
	char		*chr;
	int			idx;

	dest = copy_env(msh, dest);
	idx = 1;
	while (idx < msh->cmds->length)
	{
		arg = msh->cmds->args[idx];
		chr = ft_strchr(arg, '=');
		if (!(dest[msh->env_len + idx - 1] = (t_dict *)malloc(sizeof(t_dict))))
			exit_print_err(strerror(errno));
		if (!(dest[msh->env_len + idx - 1]->key = ft_substr(arg, 0, chr - arg)))
			exit_print_err(strerror(errno));
		if (!(dest[msh->env_len + idx - 1]->value = ft_strdup(chr + 1)))
			exit_print_err(strerror(errno));
		idx++;
	}
	dest[msh->env_len + msh->cmds->length - 1] = NULL;
		printf("%d %d\n", msh->env_len, msh->env_len + msh->cmds->length - 1);
	return (dest);
}

void			do_export(t_msh *msh, int fd)
{
	t_dict		**env_temp;
	t_cmd		*cmd;

	cmd = msh->cmds;
	if (!cmd->args[1])
	{
		if (!(env_temp = (t_dict **)malloc(
						sizeof(t_dict *) * (msh->env_len + 1))))
			exit_print_err(strerror(errno));
		env_temp = copy_env(msh, env_temp);
		quick_sort_env(0, msh->env_len - 1, env_temp);
		do_env(env_temp, fd);
		ft_double_free((void **)env_temp);
	}
	else
	{
		if (!(env_temp = (t_dict **)malloc(
						sizeof(t_dict *) * (msh->env_len + (cmd->length - 1)))))
			exit_print_err(strerror(errno));
		printf("%d = envlen%d + cmd%d\n", msh->env_len + cmd->length - 1, msh->env_len, cmd->length - 1);
		env_temp = export_env(msh, env_temp);
		ft_double_free((void **)msh->env);
		msh->env_len += (cmd->length - 1);
		msh->env = env_temp;
	}
}
