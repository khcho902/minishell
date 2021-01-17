/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/16 17:09:53 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_pwd(t_msh *msh, t_cmd *cmd)
{
	(void)cmd;
	ft_putstr_fd(msh->pwd, STDOUT);
	ft_putchar_fd('\n', STDOUT);
}
