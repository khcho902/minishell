/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 15:54:19 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		do_pwd(t_msh *msh)
{
	if (!msh->wd)
		return (EXIT_FAILURE);
	ft_putstr_fd(msh->wd, STDOUT);
	ft_putchar_fd('\n', STDOUT);
	return (EXIT_SUCCESS);
}
