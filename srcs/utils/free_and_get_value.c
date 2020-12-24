/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_get_value.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 15:29:41 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/24 15:48:50 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_get_value(char **dst, char *src)
{
	if (*dst != NULL)
		free(*dst);
	if (!(*dst = ft_strdup(src)))
		exit_print_err(strerror(errno));
}
