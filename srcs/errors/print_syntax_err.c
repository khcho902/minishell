/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_syntax_err.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 17:55:14 by kycho             #+#    #+#             */
/*   Updated: 2021/01/01 16:51:57 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		print_syntax_err(char *program_name, char *token, int is_eof_error)
{
	ft_putstr_fd(program_name, STDERR);
	if (is_eof_error == TRUE)
		ft_putstr_fd(": unexpected EOF while looking for matching `", STDERR);
	else
		ft_putstr_fd(": syntax error near unexpected token `", STDERR);
	ft_putstr_fd(token, STDERR);
	ft_putstr_fd("'\n", STDERR);
	return (ERROR);
}
