/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 06:09:04 by kycho             #+#    #+#             */
/*   Updated: 2021/01/25 02:15:06 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_piping_material(t_cmd *cmd, t_piping_material *pm)
{
	pm->cnt_of_pipes = 0;
	while (cmd)
	{
		if (cmd->type != TYPE_PIPE)
			break ;
		pm->cnt_of_pipes++;
		cmd = cmd->next;
	}
	if (!(pm->pipes = malloc(sizeof(int) * (pm->cnt_of_pipes * 2))))
		exit_print_err(strerror(errno));
	if (!(pm->cpid = malloc(sizeof(pid_t) * (pm->cnt_of_pipes + 1))))
		exit_print_err(strerror(errno));
}

void	piping_in_child(t_msh *msh, t_cmd *cmd, t_piping_material *pm, int i)
{
	t_builtin_executor	builtin_executor;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (i < pm->cnt_of_pipes)
		dup2(pm->pipes[i * 2 + 1], 1);
	if (i > 0)
		dup2(pm->pipes[(i - 1) * 2], 0);
	if (i < pm->cnt_of_pipes)
	{
		close(pm->pipes[(i * 2) + 1]);
		close(pm->pipes[(i * 2)]);
	}
	if (i > 0)
		close(pm->pipes[(i - 1) * 2]);
	builtin_executor = get_builtin_executor(cmd->args[0]);
	if (set_redirection_fd(msh, cmd) == ERROR)
		exit(1);
	if (builtin_executor)
	{
		run_builtin_executor(msh, cmd, builtin_executor, TRUE);
		exit(g_exit_status);
	}
	else
		basic_executor(msh, cmd);
}

void	all_wait_and_finish_piping(t_piping_material *pm)
{
	int status;
	int	i;

	i = 0;
	while (i < pm->cnt_of_pipes + 1)
	{
		waitpid(pm->cpid[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	free(pm->pipes);
	free(pm->cpid);
}

t_cmd	*piping(t_msh *msh, t_cmd *cmd)
{
	t_piping_material	pm;
	int					i;

	init_piping_material(cmd, &pm);
	i = 0;
	while (i < (pm.cnt_of_pipes + 1))
	{
		if (i != pm.cnt_of_pipes)
		{
			if (pipe(pm.pipes + (i * 2)) == -1)
				exit_print_err(strerror(errno));
		}
		if ((pm.cpid[i] = fork()) == -1)
			exit_print_err(strerror(errno));
		else if (pm.cpid[i] == 0)
			piping_in_child(msh, cmd, &pm, i);
		if (i < pm.cnt_of_pipes)
			close(pm.pipes[(i * 2) + 1]);
		if (i > 0)
			close(pm.pipes[(i - 1) * 2]);
		cmd = cmd->next;
		i++;
	}
	all_wait_and_finish_piping(&pm);
	return (cmd);
}
