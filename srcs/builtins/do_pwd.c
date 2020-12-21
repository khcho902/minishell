/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:10:13 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/21 20:06:28 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_pwd(t_msh *msh)
{
	char	*temp;

	if (!msh)
		return ;
	if (!(temp = getcwd(NULL, 0)))
		exit_print_err(strerror(errno));
	ft_putstr_fd(temp, STDOUT);
	ft_putchar_fd('\n', STDOUT);
	free(temp);
}
