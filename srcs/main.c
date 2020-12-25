/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 19:08:26 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*compare_arg(t_msh *msh, t_cmd *cmd)
{
	if (!msh)
		return (NULL);
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (&do_exit);
	else if (!ft_strcmp(cmd->args[0], "echo"))
		return (&do_echo);
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (&do_env);
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (&do_export);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (&do_pwd);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (&do_unset);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (&do_cd);
	else
		return (&executor);
}

void	main_loop(t_msh *msh)
{
	t_exe_fn	func;
	t_cmd		*cmd;

	cmd = msh->cmds;
	while (cmd)
	{
		if (cmd->args[0] == NULL)
			break ;
		func = compare_arg(msh, cmd);
		redirection_input_fd(cmd, cmd->redirection_files);
		redirection_output_fd(cmd, cmd->redirection_files);
		if ((void *)func != &executor && cmd->type == TYPE_DEFAULT &&
			cmd->input_fd == -1 && cmd->output_fd == -1 &&
			(cmd->prev == NULL ||
			 (cmd->prev && cmd->prev->type == TYPE_DEFAULT)))
			func(msh, cmd);
		else
			create_process(msh, cmd, func);
		cmd = cmd->next;
	}
}

int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	init_signal();
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
