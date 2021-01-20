/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:52 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/21 03:41:09 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_unset_sub(t_msh *msh, t_cmd *cmd, int i)
{
	if (ft_strcmp(cmd->args[i], "PWD") == 0)
		msh->unset_pwd_flag = 1;
	else if (ft_strcmp(cmd->args[i], "PATH") == 0)
	{
		free(msh->path);
		if (!(msh->path = ft_strdup("")))
			exit_print_err(strerror(errno));
	}
	unset_env_dict(msh, cmd->args[i]);
}

void	do_unset(t_msh *msh, t_cmd *cmd)
{
	int i;
	int res;

	res = 0;
	i = 1;
	while (i < cmd->length)
	{
		if (is_fine_env_key(cmd->args[i]) == TRUE)
			do_unset_sub(msh, cmd, i);
		else
		{
			print_identifier_err(msh->program_name, cmd->args[0], cmd->args[i]);
			res = 1;
		}
		i++;
	}
	msh->exit_status = res;
}
