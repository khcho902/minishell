/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:03:05 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 15:47:21 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_input_fd(t_cmd *cmd, t_list *list)
{
	while (list)
	{
		if (!ft_strcmp("<", list->content))
		{
			if (cmd->input_fd != -1)
				close(cmd->input_fd);
			list = list->next;
			if ((cmd->input_fd = open(list->content, FLAG_I, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		list = list->next;
	}
}

void	redirection_output_fd(t_cmd *cmd, t_list *list)
{
	while (list)
	{
		if (!ft_strcmp(">", list->content))
		{ 
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd =
						open(list->content, FLAG_O, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		else if (!ft_strcmp(">>", list->content))
		{
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd =
						open(list->content, FLAG_AO, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		list = list->next;
	}
}

int		close_fds(t_cmd *cmd, pid_t pid, int pipe_open)
{
	int		status;
	int		ret;

	ret = EXIT_SUCCESS;
	status = 0;
	waitpid(pid, &status, 0);
	if (pipe_open)
	{
		close(cmd->pipes[PIPE_IN]);
		if (!cmd->next || cmd->type == TYPE_DEFAULT)
			close(cmd->pipes[PIPE_OUT]);
	}
	if (cmd->prev && cmd->prev->type == TYPE_PIPE)
		close(cmd->prev->pipes[PIPE_OUT]);
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
	if (cmd->output_fd != -1)
		close(cmd->output_fd);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	if (ret != EXIT_SUCCESS)
		ret = EXIT_FAILURE;
	return (ret);
}

void	child_process(t_msh *msh, t_cmd *cmd, t_exe_fn func)
{
	if (cmd->type == TYPE_PIPE &&
			dup2(cmd->pipes[PIPE_IN], STDOUT) < 0)
		exit_print_err(strerror(errno));
	if (cmd->prev && cmd->prev->type == TYPE_PIPE &&
			dup2(cmd->prev->pipes[PIPE_OUT], STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->input_fd != -1 && dup2(cmd->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (cmd->output_fd != -1 && dup2(cmd->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	func(msh, cmd);
	exit(EXIT_SUCCESS);
}

int		create_process(t_msh *msh, t_cmd *cmd, t_exe_fn func)
{
	pid_t	pid;
	int		ret;
	int		pipe_open;

	pipe_open = 0;
	ret = EXIT_SUCCESS;
	if (cmd->type == TYPE_PIPE)
	{
		pipe_open = 1;
		if (pipe(cmd->pipes) == -1)
			exit_print_err(strerror(errno));
	}
	if ((pid = fork()) == -1)
		exit_print_err(strerror(errno));
	if (pid == 0)
		child_process(msh, cmd, func);
	else
		ret = close_fds(cmd, pid, pipe_open);
	return (ret);
}
