/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:35:43 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 16:21:38 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*compare_arg(t_msh *msh)
{
	if (!ft_strcmp(msh->cmds->args[0], "exit"))
		return (&do_exit);
	else if (!ft_strcmp(msh->cmds->args[0], "echo"))
		return (&do_echo);
	else if (!ft_strcmp(msh->cmds->args[0], "env"))
		return (&do_env);
	else if (!ft_strcmp(msh->cmds->args[0], "export"))
		return (&do_export);
	else if (!ft_strcmp(msh->cmds->args[0], "pwd"))
		return (&do_pwd);
	else if (!ft_strcmp(msh->cmds->args[0], "unset"))
		return (&do_unset);
	else if (!ft_strcmp(msh->cmds->args[0], "cd"))
		return (&do_cd);
	else
		return (&executor);
}
