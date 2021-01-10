/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:35:33 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/10 13:26:58 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_exit(t_msh *msh, t_cmd *cmd)
{
	char *program_name;
	char *tmp;

	program_name = msh->program_name;
	if (cmd->length == 1)
		exit(msh->exit_status);
	if (is_numeric_long_str(cmd->args[1]) == FALSE)
	{
		if (!(tmp = ft_strjoin3(cmd->args[0], ": ", cmd->args[1])))
			exit_print_err(strerror(errno));
		print_execute_err(program_name, tmp, "numeric argument required");
		msh->exit_status = 255;
		exit(msh->exit_status & 255);
	}
	if (cmd->length == 2)
		msh->exit_status = ft_atoi(cmd->args[1]);
	else
	{
		print_execute_err(program_name, cmd->args[0], "too many arguments");
		msh->exit_status = 1;
	}
	exit(msh->exit_status);
}
