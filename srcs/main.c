/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/17 19:49:50 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH 256

void		show_prompt(char *str)
{
	ft_putstr_fd(str, STDIN_FILENO);
	ft_putchar_fd(' ', STDIN_FILENO);
}

static int	cmdcmp(char *str)
{
	int			i;
	int			str_len;
	char		*name;
	const char	*cmd_list[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
	};

	i = 0;
	str_len = ft_strlen(str);
	while (cmd_list[i] && (name = (char *)cmd_list[i]))
	{
		if (ft_strncmp(str, name, str_len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int			builtins(int key, t_list *env_list)
{
	char		buf[MAX_PATH];

	if (key == k_echo)
	{
		ft_putstr_fd("echo\n", STDIN_FILENO);
	}
	else if (key == k_cd)
	{
		chdir("../");
		printf("change directory[%s]\n", getcwd(buf, MAX_PATH));
	}
	else if (key == k_pwd)
	{
		ft_putstr_fd(getcwd(buf, MAX_PATH), STDIN_FILENO);
		ft_putchar_fd('\n', STDIN_FILENO);
	}
	else if (key == k_export)
		ft_putstr_fd("export\n", STDIN_FILENO);
	else if (key == k_unset)
		ft_putstr_fd("unset\n", STDIN_FILENO);
	else if (key == k_env)
		do_env(env_list);
	else if (key == k_exit)
		exit(0);
	return (0);
}

int			main(int ac, char **av, char **env)
{
	char		*input;
	int			cmd;
	t_list		*env_list;

	env_list = init_env(env);
	while (ac)
	{
		show_prompt(av[0]);
		if (get_next_line(STDIN_FILENO, &input) > -1)
		{
			cmd = cmdcmp(input);
			if (cmd < 10)
				builtins(cmd, env_list);
			free(input);
		}
	}
}
