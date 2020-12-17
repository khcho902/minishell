/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 05:44:26 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 15:08:19 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_envjoin(t_dict **env, int env_len)
{
	char	**temp;
	int		idx;

	if (!(temp = (char **)malloc(sizeof(char *) * (env_len + 1))))
		exit_print_err(strerror(errno));
	idx = 0;
	while (idx < env_len)
	{
		if (!(temp[idx] = ft_strjoin3(env[idx]->key, "=", env[idx]->value)))
			exit_print_err(strerror(errno));
		idx++;
	}
	temp[idx] = NULL;
	return (temp);
}

void	exec_process(t_msh *msh, t_cmd *cmd, char **av, char **env)
{
	char		*temp;
	int			idx;
	int			ret;

	idx = 0;
	while (msh->path[idx])
	{
		if (!(temp = ft_strjoin3(msh->path[idx], "/", cmd->args[0])))
			exit_print_err(strerror(errno));
		ret = execve(temp, av, env);
		free(temp);
		idx++;
		if (ret == -1)
			continue ;
	}
	ft_double_free((void **)av);
	ft_double_free((void **)env);
}

void	executor(t_msh *msh, t_cmd *cmd)
{
	pid_t		pid;
	char		**av;
	char		**env;
	int			idx;

	if (!(av = (char **)malloc(sizeof(char *) * (cmd->length + 1))))
		exit_print_err(strerror(errno));
	if (!(av[0] = ft_strjoin("./", cmd->args[0])))
		exit_print_err(strerror(errno));
	idx = -1;
	while (++idx < cmd->length)
		if (!(av[idx] = ft_strdup(cmd->args[idx])))
			exit_print_err(strerror(errno));
	av[idx] = NULL;
	env = ft_envjoin(msh->env, msh->env_len);
	pid = fork();
	if (pid == 0)
	{
		exec_process(msh, cmd, av, env);
		exit(EXIT_SUCCESS);
	}
	else
		wait(NULL);
}
