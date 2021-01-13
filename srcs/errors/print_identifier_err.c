/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_identifier_err.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 22:32:08 by kycho             #+#    #+#             */
/*   Updated: 2021/01/13 22:38:53 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_identifier_err(char *program_name, char *token1, char *token2)
{
	ft_putstr_fd(program_name, STDERR);
	ft_putstr_fd(": ", STDERR);
	ft_putstr_fd(token1, STDERR);
	ft_putstr_fd(": `", STDERR);
	ft_putstr_fd(token2, STDERR);
	ft_putstr_fd("': not a valid identifier\n", STDERR);
}
