/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 02:43:20 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_pwd(t_msh *msh)
{
	char	temp[PATH_MAX + 1];

	if (!msh)
		return ;
	if (getcwd(temp, PATH_MAX) == NULL)
		exit_print_err(strerror(errno));
	ft_putstr_fd(temp, STDOUT);
	ft_putchar_fd('\n', STDOUT);
	free(temp);
}
