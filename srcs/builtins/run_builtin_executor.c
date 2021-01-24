/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin_executor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 01:00:39 by kycho             #+#    #+#             */
/*   Updated: 2021/01/25 01:17:30 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_builtin_executor(t_msh *msh, t_cmd *cmd,
				t_builtin_executor builtin_executor, int is_in_child_process)
{
	int stdin_fd;
	int stdout_fd;

	if (is_in_child_process == FALSE)
	{
		if ((stdin_fd = dup(STDIN)) < 0)
			exit_print_err(strerror(errno));
		if ((stdout_fd = dup(STDOUT)) < 0)
			exit_print_err(strerror(errno));
	}
	if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	builtin_executor(msh, cmd);
	if (is_in_child_process == FALSE)
	{
		if (dup2(stdin_fd, STDIN) < 0)
			exit_print_err(strerror(errno));
		if (dup2(stdout_fd, STDOUT) < 0)
			exit_print_err(strerror(errno));
		close(stdin_fd);
		close(stdout_fd);
	}
}
