/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 05:44:26 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 06:02:31 by jiseo            ###   ########.fr       */
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
	while (cmd_list[i] && (name = (char *)cmd_list[i]))
	{
		if (!ft_strcmp(str, name))
			return (i + 1);
		i++;
	}
	return (EXEC_IDX);
}

char		**ft_envjoin(t_list *env_list)
{
	char	**temp;
	int		list_size;
	int		idx;
	t_dict	*dict;
	t_list	*l;

	list_size = ft_lstsize(env_list);
	l = env_list;
	temp = (char **)malloc(sizeof(char *) * (list_size + 1));
	idx = 0;
	while (idx < list_size)
	{
		dict = l->content;
		temp[idx++] = ft_strjoin3(dict->key, "=", dict->value);
		l = l->next;
	}
	return (temp);
}

void	exec_process(t_msh *msh)
{
	char		**paths;
	t_list		*l;
	t_dict		*dict;
	char		*temp;
	char		**av;
	char		**env;
	int			i;
	
	l = msh->env_list;
	while (l)
	{
		dict = l->content;
		if (!ft_strncmp(dict->key, "PATH", 4))
		{
			paths = ft_split(&dict->value[5], ':');
			break ;
		}
		l = l->next;
	}
	i = 0;
	av = (char **)malloc(sizeof(char *));
	av[0] = ft_strjoin("./", msh->cmd_list[msh->cmd_idx]);
	env = ft_envjoin(msh->env_list);
	while (paths[i])
	{
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin(paths[i], "/");
		temp = ft_strjoin(paths[i], msh->cmd_list[msh->cmd_idx]);
		if (execve(temp, av, env) != -1)
			free(temp);
		i++;
		free(temp);
	}
	ft_double_free(av);
	ft_double_free(env);
	ft_double_free(paths);
}

void	create_process(t_msh *msh)
{
	pid_t		pid;

	pid = fork();
	if (pid == 0)
	{
		exec_process(msh);
		exit(0);
	}
	else
		wait(NULL);
}

void	main_loop(t_msh *msh)
{
	msh->cmd_list = ft_split(msh->input, ' ');
	msh->cmd_idx = 0;
	while (msh->cmd_list[msh->cmd_idx])
	{
		msh->cmd_key = builtin_compare(msh->cmd_list[msh->cmd_idx]);
		if (msh->cmd_key == EXEC_IDX)
			create_process(msh);
		else
			builtins(msh);
		msh->cmd_idx++;
	}
}

void	init_main(t_msh *msh, char **env)
{
	show_logo();
	msh->env_list = init_env(env);
	msh->wd = getcwd(NULL, 0);
	msh->prompt = ft_strdup("$USER@$NAME");
}
