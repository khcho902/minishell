/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_not_found.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 04:02:54 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 05:11:35 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		command_not_found(char *str)
{
	char	*temp;

	temp = ft_strjoin(str, ": ", "command not found.\n");
	ft_putstr_fd(temp, STDERR);
	free(temp);
}
