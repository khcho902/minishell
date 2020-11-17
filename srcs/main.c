/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/17 12:34:34 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH 256

void	show_prompt(char *str)
{
	ft_putstr_fd(str, STDIN_FILENO);
	ft_putchar_fd(' ', STDIN_FILENO);
}

t_list	*init_env(char **env)
{
	t_list	*l;
	int		i;

	l = NULL;
	i = 0;
	while (env[i])
	{
		if (!l)
			l = ft_lstnew(env[i]);
		else
			ft_lstadd_back(&l, ft_lstnew(env[i]));
		i++;
	}
	return (l);
}

void	do_env(t_list *l)
{
	t_list	*l_cpy;

	l_cpy = l;
	while (l_cpy)
	{
		ft_putstr_fd(l_cpy->content, STDIN_FILENO);
		l_cpy = l_cpy->next;
	}
}

int		main(int ac, char **av, char **env)
{
	char	*input;
	char	buf[MAX_PATH];
	char	**split;
	int		i;
	t_list	*env_list;

	env_list = init_env(env);
	while (ac)
	{
		show_prompt(av[0]);
		if (get_next_line(STDIN_FILENO, &input) > -1)
		{
			i = 0;
			split = ft_split(input, ' ');
			while (split[i])
			{
				printf("[%d]: [%s]\n", i, split[i]);
				i++;
			}
			if (ft_strncmp(input, "pwd", 3) == 0)
				printf("pwd[%s]\n", getcwd(buf, MAX_PATH));
			else if (ft_strncmp(input, "cd", 2) == 0)
			{
				chdir("/Users/jiseo/work");
				printf("change directory[%s]\n", getcwd(buf, MAX_PATH));
			}
			else if (ft_strncmp(input, "exit", 4) == 0)
				exit(0);
			else if (ft_strncmp(input, "echo", 4) == 0)
				printf("input:[%s]\n", input);
			else if (ft_strncmp(input, "env", 3) == 0)
				do_env(env_list);
			printf("split %p input %p\n", split, input);
			ft_double_free(split);
			free(input);
			printf("split %p input %p\n", split, input);
		}
	}
}
