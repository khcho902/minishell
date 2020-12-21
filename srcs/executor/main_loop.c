/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:03:05 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/21 20:19:39 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_input_fd(t_msh *msh, t_list *list)
{
	while (list)
	{
		if (!ft_strcmp("<", list->content))
		{
			if (msh->cmds->input_fd != -1)
				close(msh->cmds->input_fd);
			if ((list = list->next) && (msh->cmds->input_fd =
					open(list->content, FLAG_I, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		list = list->next;
	}
}

void	redirection_output_fd(t_msh *msh, t_list *list)
{
	while (list)
	{
		if (!ft_strcmp(">", list->content))
		{ 
			if (msh->cmds->output_fd != -1)
				close(msh->cmds->output_fd);
			if ((list = list->next) && (msh->cmds->output_fd =
					open(list->content, FLAG_O, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		else if (!ft_strcmp(">>", list->content))
		{
			if (msh->cmds->output_fd != -1)
				close(msh->cmds->output_fd);
			if ((list = list->next) && (msh->cmds->output_fd =
					open(list->content, FLAG_AO, OPEN_MODE)) == -1)
				exit_print_err(strerror(errno));
		}
		list = list->next;
	}
}

int		close_fds(t_msh *msh, pid_t pid, int pipe_open)
{
	int		status;
	int		ret;

	ret = EXIT_SUCCESS;
	status = 0;
	waitpid(pid, &status, 0);
	if (pipe_open)
	{
		close(msh->cmds->pipes[PIPE_IN]);
		if (!msh->cmds->next || msh->cmds->type == TYPE_DEFAULT)
			close(msh->cmds->pipes[PIPE_OUT]);
	}
	if (msh->cmds->prev && msh->cmds->prev->type == TYPE_PIPE)
		close(msh->cmds->prev->pipes[PIPE_OUT]);
	if (msh->cmds->input_fd != -1)
		close(msh->cmds->input_fd);
	if (msh->cmds->output_fd != -1)
		close(msh->cmds->output_fd);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	if (ret != EXIT_SUCCESS)
		ret = EXIT_FAILURE;
	return (ret);
}

void	child_process(t_msh *msh, t_exe_fn func)
{
	if (msh->cmds->type == TYPE_PIPE &&
			dup2(msh->cmds->pipes[PIPE_IN], STDOUT) < 0)
		exit_print_err(strerror(errno));
	if (msh->cmds->prev && msh->cmds->prev->type == TYPE_PIPE &&
			dup2(msh->cmds->prev->pipes[PIPE_OUT], STDIN) < 0)
		exit_print_err(strerror(errno));
	if (msh->cmds->input_fd != -1 && dup2(msh->cmds->input_fd, STDIN) < 0)
		exit_print_err(strerror(errno));
	if (msh->cmds->output_fd != -1 && dup2(msh->cmds->output_fd, STDOUT) < 0)
		exit_print_err(strerror(errno));
	func(msh);
	exit(EXIT_SUCCESS);
}

int		create_process(t_msh *msh, t_exe_fn func)
{
	pid_t	pid;
	int		ret;
	int		pipe_open;

	pipe_open = 0;
	ret = EXIT_SUCCESS;
	if (msh->cmds->type == TYPE_PIPE)
	{
		pipe_open = 1;
		if (pipe(msh->cmds->pipes) == -1)
			exit_print_err(strerror(errno));
	}
	if ((pid = fork()) == -1)
		exit_print_err(strerror(errno));
	if (pid == 0)
		child_process(msh, func);
	else
		ret = close_fds(msh, pid, pipe_open);
	return (ret);
}
