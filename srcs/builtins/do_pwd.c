/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 15:35:52 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_pwd(t_msh *msh, t_cmd *cmd)
{
	char	temp[BUFFER_SIZE + 1];

	if (!msh)
		return ;
	if (getcwd(temp, BUFFER_SIZE) == NULL)
		exit_print_err(strerror(errno));
	ft_putstr_fd(temp, STDOUT);
	ft_putchar_fd('\n', STDOUT);
	(void)cmd;
}
