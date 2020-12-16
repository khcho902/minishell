/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 06:12:53 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 06:14:34 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_print_err(char *err_msg1, char *err_msg2, int exit_status)
{
	ft_putstr_fd(err_msg1, STDERR);
	ft_putstr_fd(" : ", STDERR);
	ft_putstr_fd(err_msg2, STDERR);
	ft_putstr_fd("\n", STDERR);
	exit(exit_status);
}

int		print_syntax_err(char *program_name, char *token)
{
	ft_putstr_fd("-", STDERR);
	ft_putstr_fd(program_name, STDERR);
	ft_putstr_fd(": syntax error near unexpected token `", STDERR);
	ft_putstr_fd(token, STDERR);
	ft_putstr_fd("'\n", STDERR);
	return (ERROR);
}
