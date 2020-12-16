/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_syntax_err.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 17:55:14 by kycho             #+#    #+#             */
/*   Updated: 2020/12/16 18:04:07 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		print_syntax_err(char *program_name, char *token)
{
	ft_putstr_fd("-", STDERR);
	ft_putstr_fd(program_name, STDERR);
	ft_putstr_fd(": syntax error near unexpected token `", STDERR);
	ft_putstr_fd(token, STDERR);
	ft_putstr_fd("'\n", STDERR);
	return (ERROR);
}
