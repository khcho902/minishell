/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:52 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/14 00:18:02 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_unset(t_msh *msh, t_cmd *cmd)
{
	int i;
	int res;

	res = 0;
	i = 1;
	while (i < cmd->length)
	{
		if (is_fine_env_key(cmd->args[i]) == TRUE)
		{
			if (ft_strcmp(cmd->args[i], "PWD") == 0)
			{
				free(msh->pwd);
				if (!(msh->pwd = ft_strdup("")))
					exit_print_err(strerror(errno));
			}
			else if (ft_strcmp(cmd->args[i], "OLDPWD") == 0)
			{
				free(msh->oldpwd);
				if (!(msh->oldpwd = ft_strdup("")))
					exit_print_err(strerror(errno));
			}
			unset_env_dict(msh, cmd->args[i]);
		}
		else
		{
			print_identifier_err(msh->program_name, cmd->args[0], cmd->args[i]);
			res = 1;
		}
		i++;
	}
	msh->exit_status = res;
}
