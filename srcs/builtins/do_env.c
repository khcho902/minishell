/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:00:35 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/12 15:18:44 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_msh *msh, t_dict **env_arr, int env_len, char *command)
{
	char	**temp;
	int		idx;

	if (!env_arr)
	{
		print_execute_err(msh->program_name, command, "command not found");
//		command_not_found(msh->program_name, command);

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
	/*
	print_env(msh, msh->env, msh->env_len, "env");
	(void)cmd;
	*/
	(void)cmd;

	int i;

	i = 0;
	
	//dprintf(2, "env_len : %d\n", msh->env_len);

	while (msh->env[i])
	{
		if (msh->env[i]->value != NULL)
		{
			ft_putstr_fd(msh->env[i]->key, STDOUT);
			ft_putstr_fd("=", STDOUT);
			ft_putstr_fd(msh->env[i]->value, STDOUT);
			ft_putchar_fd('\n', STDOUT);
		}
		i++;
	}


}
