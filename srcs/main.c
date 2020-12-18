/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 19:17:32 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main_loop(t_msh *msh)
{
	pid_t			pid;
	int				ret;
	int				status;
	int				pipe_open;
	t_execute_func	func;

	ret = EXIT_SUCCESS;
	pipe_open = 0;
	while (msh->cmds)
	{
		func = compare_arg(msh);
		if (func != &executor && msh->cmds->type != TYPE_PIPE)
			ret = func(msh);
		else
		{
			if (msh->cmds->type == TYPE_PIPE)
			{
				pipe_open = 1;
				if (pipe(msh->cmds->pipes) == -1)
					exit_print_err(strerror(errno));
			}
			if ((pid = fork()) == -1)
				exit_print_err(strerror(errno));
			if (pid == 0)
			{
				if (msh->cmds->type == TYPE_PIPE &&
						dup2(msh->cmds->pipes[PIPE_IN], STDOUT) < 0)
					exit_print_err(strerror(errno));
				if (msh->cmds->prev && msh->cmds->prev->type == TYPE_PIPE &&
						dup2(msh->cmds->prev->pipes[PIPE_OUT], STDIN) < 0)
					exit_print_err(strerror(errno));
				ret = func(msh);
				exit(ret);
			}
			else
			{
				waitpid(pid, &status, 0);
				if (pipe_open)
				{
					close(msh->cmds->pipes[PIPE_IN]);
					if (!msh->cmds->next || msh->cmds->type == TYPE_DEFAULT)
						close(msh->cmds->pipes[PIPE_OUT]);
				}
				if (msh->cmds->prev && msh->cmds->prev->type == TYPE_PIPE)
					close(msh->cmds->prev->pipes[PIPE_OUT]);
				if (WIFEXITED(status))
					ret = WEXITSTATUS(status);
			}
		}
		msh->cmds = msh->cmds->next;
	}
	return (ret);
}

int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	res = argc;
	while (res)
	{
		show_prompt(&msh);
		res = get_next_line(STDIN, &input);
		if (res == -1)
			exit_print_err("get_next_line fail!");
		else
		{
			if (parsing(&msh, input) == SUCCESS)
				main_loop(&msh);
			free_msh_member(&msh);
			free(input);
		}
	}
	return (0);
}
