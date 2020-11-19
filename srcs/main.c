/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/20 03:23:46 by jiseo            ###   ########.fr       */
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

void		do_pwd(int fd)
{
	char	buf[MAX_PATH];

	ft_putstr_fd(getcwd(buf, MAX_PATH), fd);
	ft_putchar_fd('\n', fd);
}

int			builtins(t_msh *msh)
{
	if (msh->cmd_key == k_echo)
	{
		ft_putstr_fd("echo\n", STDIN_FILENO);
	}
	else if (msh->cmd_key == k_cd)
		chdir(msh->cmd_list[++msh->cmd_idx]);
	else if (msh->cmd_key == k_pwd) // complete
		do_pwd(STDIN_FILENO);
	else if (msh->cmd_key == k_export)
		ft_putstr_fd("export\n", STDIN_FILENO);
	else if (msh->cmd_key == k_unset)
		ft_putstr_fd("unset\n", STDIN_FILENO);
	else if (msh->cmd_key == k_env) // complete
		do_env(msh->env_list, STDIN_FILENO);
	else if (msh->cmd_key == k_exit)
		exit(0);
	return (0);
}

void		main_loop(char *input, t_msh *msh)
{
	msh->cmd_list = ft_split(input, ' ');
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
	char		*input;
	t_msh		msh;

	msh.env_list = init_env(env);
	while (ac)
	{
		show_prompt(av[0]);
		if (get_next_line(STDIN_FILENO, &input) == -1)
			break ;
		main_loop(input, &msh);
		free(input);
	}
	return (0);
}
