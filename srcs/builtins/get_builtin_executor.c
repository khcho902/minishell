/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_builtin_executor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 18:56:08 by kycho             #+#    #+#             */
/*   Updated: 2021/01/17 18:20:52 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*get_builtin_executor(char *cmd_name)
{
	if (cmd_name == NULL)
		return (NULL);
	if (!ft_strcmp(cmd_name, "exit"))
		return (&do_exit);
	else if (!ft_strcmp(cmd_name, "echo"))
		return (&do_echo);
	else if (!ft_strcmp(cmd_name, "env"))
		return (&do_env);
	else if (!ft_strcmp(cmd_name, "export"))
		return (&do_export);
	else if (!ft_strcmp(cmd_name, "pwd"))
		return (&do_pwd);
	else if (!ft_strcmp(cmd_name, "unset"))
		return (&do_unset);
	else if (!ft_strcmp(cmd_name, "cd"))
		return (&do_cd);
	return (NULL);
}
