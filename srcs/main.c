/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/17 18:47:48 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH 256

void		show_prompt(char *str)
{
	ft_putstr_fd(str, STDIN_FILENO);
	ft_putchar_fd(' ', STDIN_FILENO);
}

t_kv		*key_value_generator(char *env)
{
	t_kv	*kv;
	char	*chr;
	int		i;

	i = 0;
	if (!(kv = (t_kv *)malloc(sizeof(t_kv))))
		return (NULL);
	chr = ft_strchr(env, '=');
	kv->key = ft_substr(env, 0, chr - env);
	kv->value = chr + 1;
	return (kv);
}

t_list		*init_env(char **env)
{
	t_list	*l;
	int		i;

	l = NULL;
	i = 0;
	while (env[i])
	{
		if (!l)
			l = ft_lstnew(key_value_generator(env[i]));
		else
			ft_lstadd_back(&l, ft_lstnew(key_value_generator(env[i])));
		i++;
	}
	return (l);
}

void		do_env(t_list *l)
{
	t_list	*l_cpy;
	t_kv	*kv;

	l_cpy = l;
	while (l_cpy)
	{
		kv = l_cpy->content;
		ft_putstr_fd(kv->key, STDIN_FILENO);
		ft_putchar_fd('=', STDIN_FILENO);
		ft_putstr_fd(kv->value, STDIN_FILENO);
		ft_putchar_fd('\n', STDIN_FILENO);
		l_cpy = l_cpy->next;
	}
}

/*
static char	*cmd_generator(char *str)
{
	int			i;
	int			str_len;
	char		*name;
	const char	*cmd_list[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};

	i = 0;
	str_len = ft_strlen(str);
	if (cmd_list[i] && (name = (char *)cmd_list[i]))
	{
		if (ft_strncmp(str, name, str_len) == 0)
			return (ft_strdup(name));
		i++;
	}
	return (NULL);
}*/

int			main(int ac, char **av, char **env)
{
	char		*input;
	char		buf[MAX_PATH];
	char		**split;
	int			i;
	t_list		*env_list;

	env_list = init_env(env);
	while (ac)
	{
		show_prompt(av[0]);
		if (get_next_line(STDIN_FILENO, &input) > -1)
		{
			i = 0;
			split = ft_split(input, ' ');
			if (ft_strncmp(input, "pwd", 3) == 0)
				printf("pwd[%s]\n", getcwd(buf, MAX_PATH));
			else if (ft_strncmp(input, "cd", 2) == 0)
			{
				chdir("../");
				printf("change directory[%s]\n", getcwd(buf, MAX_PATH));
			}
			else if (ft_strncmp(input, "exit", 4) == 0)
				exit(0);
			else if (ft_strncmp(input, "echo", 4) == 0)
				printf("input:[%s]\n", input);
			else if (ft_strncmp(input, "env", 3) == 0)
				do_env(env_list);
			ft_double_free(split);
			free(input);
		}
	}
}
