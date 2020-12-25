/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:00:35 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 15:34:37 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_msh *msh, t_dict **env_arr, int env_len, char *command)
{
	char	**temp;
	int		idx;

	if (!env_arr)
	{
		command_not_found(msh->program_name, command);
		return ;
	}
	idx = 0;
	temp = ft_envjoin(env_arr, env_len);
	if (!temp)
		exit_print_err(strerror(errno));
	while (temp[idx])
	{
		ft_putstr_fd(temp[idx], STDOUT);
		ft_putchar_fd('\n', STDOUT);
		idx++;
	}
	ft_double_free((void **)temp);
}

void	do_env(t_msh *msh, t_cmd *cmd)
{
	print_env(msh, msh->env, msh->env_len, "env");
	(void)cmd;
}
