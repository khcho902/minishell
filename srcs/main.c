/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/25 22:44:43 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			builtins(t_msh *msh)
{
	if (msh->cmd_key == k_echo)
		do_echo(msh, STDIN_FILENO);
	else if (msh->cmd_key == k_cd)
		do_cd(msh);
	else if (msh->cmd_key == k_pwd)
		do_pwd(STDIN_FILENO);
	else if (msh->cmd_key == k_export)
		do_export(msh);
	else if (msh->cmd_key == k_unset)
		do_unset(msh);
	else if (msh->cmd_key == k_env)
		do_env(msh->env_list, STDOUT);
	else if (msh->cmd_key == k_exit)
		exit(0);
	return (0);
}

static int	cmdcmp(char *str)
{
	int			i;
	char		*name;
	const char	*cmd_list[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
	};

	i = 0;
	while (cmd_list[i] && (name = (char *)cmd_list[i]))
	{
		if (!ft_strncmp(str, name, ft_max(ft_strlen(str), ft_strlen(name))))
			return (i);
		i++;
	}
	return (-1);
}

void		main_loop(t_msh *msh)
{
	msh->cmd_list = ft_split(msh->input, ' ');
	msh->cmd_idx = 0;
	while (msh->cmd_list[msh->cmd_idx])
	{
		msh->cmd_key = cmdcmp(msh->cmd_list[msh->cmd_idx]) + 1;
		if (msh->cmd_key < 10)
			builtins(msh);
		msh->cmd_idx++;
	}
}

int			main(int ac, char **av, char **env)
{
	t_msh		msh;

	msh.env_list = init_env(env);
	while (ac)
	{
		show_prompt(av[0]);
		if (get_next_line(STDIN, &(msh.input)) == -1)
			break ;
		main_loop(&msh);
		free(msh.input);
	}
	return (0);
}
