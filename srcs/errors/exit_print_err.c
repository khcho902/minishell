/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_print_err.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 17:54:41 by kycho             #+#    #+#             */
/*   Updated: 2020/12/16 17:55:57 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_print_err(char *err_msg)
{
	ft_putstr_fd("Error : ", STDERR);
	ft_putstr_fd(err_msg, STDERR);
	ft_putstr_fd("\n", STDERR);
	exit(EXIT_FAILURE);
}
