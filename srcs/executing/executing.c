/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:03:05 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/25 02:37:35 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_lstcmd(t_msh *msh, t_cmd *cmd)
{
	free(msh->lstcmd);
	if (cmd->length == 0)
	{
		if (!(msh->lstcmd = ft_strdup("")))
			exit_print_err(strerror(errno));
	}
	else
	{
		if (!(msh->lstcmd = ft_strdup(cmd->args[cmd->length - 1])))
			exit_print_err(strerror(errno));
	}
}

void	executing_sub(t_msh *msh, t_cmd *cmd)
{
	t_builtin_executor	builtin_executor;

	builtin_executor = get_builtin_executor(cmd->args[0]);
	if (set_redirection_fd(msh, cmd) == ERROR)
	{
		if (cmd->input_fd != -1)
			close(cmd->input_fd);
		if (cmd->output_fd != -1)
			close(cmd->output_fd);
		g_exit_status = 1;
		return ;
	}
	if (builtin_executor)
		run_builtin_executor(msh, cmd, builtin_executor, FALSE);
	else
		create_process(msh, cmd);
}

void	executing(t_msh *msh)
{
	t_cmd		*cmd;

	cmd = msh->cmds;
	while (cmd)
	{
		set_lstcmd(msh, cmd);
		if (cmd->type == TYPE_PIPE)
		{
			cmd = piping(msh, cmd);
			continue ;
		}
		executing_sub(msh, cmd);
		cmd = cmd->next;
		unset_env_dict(msh, "_");
	}
}
