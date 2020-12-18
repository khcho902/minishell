/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 17:57:01 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main_loop(t_msh *msh)
{
	pid_t			pid;
	int				ret;
	t_execute_func	func;

	ret = EXIT_SUCCESS;
	while (msh->cmds)
	{
		func = compare_arg(msh);
		if (func != &executor && msh->cmds->type != TYPE_PIPE)
			ret = func(msh);
		else
		{
			if (msh->cmds->type == TYPE_PIPE)
				if (pipe(msh->cmds->pipes) == -1)
					exit_print_err(strerror(errno));
			if ((pid = fork()) == -1)
				exit_print_err(strerror(errno));
			if (pid == 0)
			{
				ret = func(msh);
				exit(ret);
			}
			else
				wait(NULL);
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
