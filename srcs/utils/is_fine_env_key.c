/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_fine_env_key.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 22:45:38 by kycho             #+#    #+#             */
/*   Updated: 2021/01/13 22:46:01 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_fine_env_key(char *key)
{
	int i;

	if (ft_strlen(key) == 0)
		return (FALSE);
	if (!(key[0] == '_' || ('a' <= key[0] && key[0] <= 'z') ||
				('A' <= key[0] && key[0] <= 'Z')))
		return (FALSE);
	i = 0;
	while (key[i])
	{
		if (!((key[i] == '_') || ('a' <= key[i] && key[i] <= 'z') ||
			('A' <= key[i] && key[i] <= 'Z') ||
			('0' <= key[i] && key[i] <= '9')))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
