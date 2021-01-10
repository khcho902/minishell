/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_shlvl_err.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 14:15:03 by kycho             #+#    #+#             */
/*   Updated: 2021/01/10 14:17:19 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_shlvl_err(char *program_name, char *value)
{
	ft_putstr_fd(program_name, STDERR);
	ft_putstr_fd(": warning: shell level (", STDERR);
	ft_putstr_fd(value, STDERR);
	ft_putstr_fd(") too high, resetting to 1\n", STDERR);
}
