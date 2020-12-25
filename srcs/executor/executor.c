/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 05:44:26 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 15:37:46 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_process(t_msh *msh, t_cmd *cmd, char **av, char **env)
{
	char	*temp;
	int		idx;
	int		ret;

	idx = 0;
	while (msh->path[idx])
	{
		if (!(temp = ft_strjoin3(msh->path[idx], "/", cmd->args[0])))
			exit_print_err(strerror(errno));
		ret = execve(temp, av, env);
		free(temp);
		idx++;
	}
	if (ret == -1)
		ret = command_not_found(msh->program_name, cmd->args[0]);
	ft_double_free((void **)av);
	ft_double_free((void **)env);
	return (ret);
}

int			executor(t_msh *msh, t_cmd *cmd)
{
	char	**av;
	char	**env;
	int		idx;

	if (!(av = (char **)malloc(sizeof(char *) * (cmd->length + 1))))
		exit_print_err(strerror(errno));
	if (!(av[0] = ft_strjoin("./", cmd->args[0])))
		exit_print_err(strerror(errno));
	idx = 1;
	while (idx < cmd->length)
	{
		if (!(av[idx] = ft_strdup(cmd->args[idx])))
			exit_print_err(strerror(errno));
		idx++;
	}
	av[idx] = NULL;
	env = ft_envjoin(msh->env, msh->env_len);
	return (exec_process(msh, cmd, av, env));
}
