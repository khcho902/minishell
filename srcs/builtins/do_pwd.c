/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/12 14:20:02 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_pwd(t_msh *msh, t_cmd *cmd)
{
	/*
	(void)cmd;
	ft_putstr_fd(msh->pwd, STDOUT);
	ft_putchar_fd('\n', STDOUT);
	*/
	char	temp[BUFFER_SIZE + 1];

	if (!msh)
		return ;
	if (getcwd(temp, BUFFER_SIZE) == NULL)
		exit_print_err(strerror(errno));
	ft_putstr_fd(temp, STDOUT);
	ft_putchar_fd('\n', STDOUT);
	(void)cmd;
}
