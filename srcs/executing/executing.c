/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:03:05 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/21 05:59:04 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executing(t_msh *msh)
{
	t_builtin_executor	builtin_executor;
	t_cmd		*cmd;
	int stdin_fd;
	int stdout_fd;

	cmd = msh->cmds;
	while (cmd)
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

		if (cmd->type == TYPE_PIPE)
		{
			cmd = piping(msh, cmd);
			continue ;
		}
		
		builtin_executor = get_builtin_executor(cmd->args[0]);
		

		if (set_redirection_fd(msh, cmd) == ERROR)
		{
			if (cmd->input_fd != -1)
				close(cmd->input_fd);
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			cmd = cmd->next;
			g_exit_status = 1;
			continue;
		}

		if (builtin_executor)
		{
			if ((stdin_fd = dup(STDIN)) < 0)
				exit_print_err(strerror(errno));
			if ((stdout_fd = dup(STDOUT)) < 0)
				exit_print_err(strerror(errno));

			if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
				exit_print_err(strerror(errno));
			if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
				exit_print_err(strerror(errno));

			builtin_executor(msh, cmd);

			if (dup2(stdin_fd, STDIN) < 0)
				exit_print_err(strerror(errno));
			if (dup2(stdout_fd, STDOUT) < 0)
				exit_print_err(strerror(errno));

			close(stdin_fd);
			close(stdout_fd);
		}
		else
		{
			create_process(msh, cmd);
		}

		cmd = cmd->next;

		unset_env_dict(msh, "_");
	}
}
