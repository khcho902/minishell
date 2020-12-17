/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 15:06:35 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		builtin_compare(char *str)
{
	int			i;
	char		*name;
	const char	*cmd_list[] = {
		"cd", "echo", "env", "export", "pwd", "unset", "exit", NULL
	};

	i = 0;
	while (cmd_list[i])
	{
		name = (char *)cmd_list[i];
		if (!ft_strcmp(str, name))
			return (i + 1);
		i++;
	}
	return (EXEC_IDX);
}

void	main_loop(t_msh *msh)
{
	t_cmd	*cmd;
	int		cmd_key;

	cmd = msh->cmds;
	while (cmd)
	{
		cmd_key = builtin_compare(cmd->args[0]);
		if (cmd_key == EXEC_IDX)
			executor(msh, cmd);
		else if (cmd_key > EXEC_IDX)
			builtins(msh, cmd_key);
		cmd = cmd->next;
	}
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
