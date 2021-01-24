/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 06:07:06 by kycho             #+#    #+#             */
/*   Updated: 2021/01/24 16:56:16 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_process(t_msh *msh, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) == -1)
		exit_print_err(strerror(errno));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		basic_executor(msh, cmd);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		if (cmd->input_fd != -1)
			close(cmd->input_fd);
		if (cmd->output_fd != -1)
			close(cmd->output_fd);
	}
}
