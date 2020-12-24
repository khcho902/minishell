/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/24 16:17:26 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*compare_arg(t_msh *msh)
{
	if (!ft_strcmp(msh->cmds->args[0], "exit"))
		return (&do_exit);
	else if (!ft_strcmp(msh->cmds->args[0], "echo"))
		return (&do_echo);
	else if (!ft_strcmp(msh->cmds->args[0], "env"))
		return (&do_env);
	else if (!ft_strcmp(msh->cmds->args[0], "export"))
		return (&do_export);
	else if (!ft_strcmp(msh->cmds->args[0], "pwd"))
		return (&do_pwd);
	else if (!ft_strcmp(msh->cmds->args[0], "unset"))
		return (&do_unset);
	else if (!ft_strcmp(msh->cmds->args[0], "cd"))
		return (&do_cd);
	else
		return (&executor);
}

void	main_loop(t_msh *msh)
{
	t_exe_fn	func;

	func = NULL;
	while (msh->cmds)
	{
		if (msh->cmds->args[0] == NULL)
			break ;
		func = compare_arg(msh);
		redirection_input_fd(msh, msh->cmds->redirection_files);
		redirection_output_fd(msh, msh->cmds->redirection_files);
		if ((void *)func != &executor && msh->cmds->type == TYPE_DEFAULT &&
			msh->cmds->input_fd == -1 && msh->cmds->output_fd == -1 &&
			(msh->cmds->prev == NULL ||
			(msh->cmds->prev && msh->cmds->prev->type == TYPE_DEFAULT)))
			func(msh);
		else
			create_process(msh, func);
		msh->cmds = msh->cmds->next;
	}
}

int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	show_logo();
	res = argc;
	while (res)
	{
		show_prompt(&msh);
		res = get_next_line(STDIN, &input);
		if (res == -1)
			exit_print_err("get_next_line fail");
		else
		{
			if (parsing(&msh, input) == SUCCESS)
				main_loop(&msh);
			free_msh_member(&msh);
			free(input);
		}
	}
	return (EXIT_SUCCESS);
}
