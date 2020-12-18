/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 11:55:29 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 12:05:39 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		print_err(char *where, char *arg, char *msg)
{
	char	*temp;

	temp = ft_strjoin3("-minishell: ", where, ": ");
	ft_putstr_fd(temp, STDERR);
	free(temp);
	temp = ft_strjoin3(arg, ": ", msg);
	ft_putstr_fd(temp, STDERR);
	free(temp);
	ft_putchar_fd('\n', STDERR);
	return (EXIT_FAILURE);
}
