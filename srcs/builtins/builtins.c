/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:35:43 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 12:39:38 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		builtins(t_msh *msh, int cmd_key)
{
	if (cmd_key == k_cd)
		do_cd(msh);
	else if (cmd_key == k_echo)
		do_echo(msh, STDOUT);
	else if (cmd_key == k_env)
		do_env(msh->env, STDOUT);
	else if (cmd_key == k_export)
		do_export(msh, STDOUT);
	else if (cmd_key == k_pwd)
		do_pwd(msh, STDOUT);
	else if (cmd_key == k_unset)
		do_unset(msh);
	else if (cmd_key == k_exit)
		exit(0);
	return (0);
}
