/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 06:09:04 by kycho             #+#    #+#             */
/*   Updated: 2021/01/21 06:09:19 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_cnt_of_pipes(t_cmd *cmd)
{
	int cnt;

	cnt = 0;
	while (cmd)
	{
		if (cmd->type != TYPE_PIPE)
			break;
		cnt++;
		cmd = cmd->next;
	}
	return (cnt);
}

void all_wait(int *cpid, int cnt_of_pipes)
{
	int status;
	int	i;

	i = 0;
	while (i < cnt_of_pipes + 1)
	{
		waitpid(cpid[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}


t_cmd	*piping(t_msh *msh, t_cmd *cmd)
{
	t_builtin_executor	builtin_executor;
	int cnt_of_pipes;
	int *pipes;
	int *cpid;
	int i;

	cnt_of_pipes = get_cnt_of_pipes(cmd);
	if (!(pipes = malloc(sizeof(int) * (cnt_of_pipes * 2))))
		exit_print_err(strerror(errno));
	if (!(cpid = malloc(sizeof(pid_t) * (cnt_of_pipes + 1))))
		exit_print_err(strerror(errno));

	i = 0;
	while (i < (cnt_of_pipes + 1))
	{
		// create 
		if (i != cnt_of_pipes)
		{
			if (pipe(pipes + (i * 2)) == -1)
				exit_print_err(strerror(errno));
		}
		if ((cpid[i] = fork()) == -1)
			exit_print_err(strerror(errno));
		else if (cpid[i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);

			if (i < cnt_of_pipes)
				dup2(pipes[i * 2 + 1], 1);
			if (i > 0)
				dup2(pipes[(i - 1) * 2], 0);
			if (i < cnt_of_pipes)
			{
				close(pipes[(i * 2) + 1]);
				close(pipes[(i * 2)]);
			}
			if (i > 0)
				close(pipes[(i - 1) * 2]);

			
			builtin_executor  = get_builtin_executor(cmd->args[0]);	
			if (set_redirection_fd(msh, cmd) == ERROR)
				exit(1);
			if (builtin_executor)
			{
				if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
					exit_print_err(strerror(errno));
				if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
					exit_print_err(strerror(errno));
				builtin_executor(msh, cmd);
				exit(g_exit_status);
			}
			else
				basic_executor(msh, cmd);
		}

		// close 
		if (i < cnt_of_pipes)
			close(pipes[(i * 2) + 1]);
		if (i > 0)
			close(pipes[(i - 1) * 2]);

		cmd = cmd->next;
		i++;
	}
	all_wait(cpid, cnt_of_pipes);
	free(pipes);
	free(cpid);
	return (cmd);
}
