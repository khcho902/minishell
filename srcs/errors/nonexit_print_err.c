/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonexit_print_err.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 14:56:03 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/24 14:56:41 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	nonexit_print_err(char *err_msg)
{
	ft_putstr_fd("Error : ", STDERR);
	ft_putstr_fd(err_msg, STDERR);
	ft_putchar_fd('\n', STDERR);
}
