/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_not_found.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 04:02:54 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/23 19:27:30 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		command_not_found(char *program_name, char *cmd)
{
	char	*temp;

	temp = ft_strjoin3("-", program_name, ": ");
	ft_putstr_fd(temp, STDERR);
	free(temp);
	temp = ft_strjoin3(cmd, ": ", "command not found\n");
	ft_putstr_fd(temp, STDERR);
	free(temp);
	return (EXIT_FAILURE);
}
