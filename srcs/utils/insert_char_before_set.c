/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_char_before_set.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 21:07:59 by kycho             #+#    #+#             */
/*   Updated: 2021/01/13 21:57:42 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*insert_char_before_set(char *str, char *set, char ch)
{
	int		i;
	char	*tmp;

	if (!(tmp = ft_strdup("")))
		exit_print_err(strerror(errno));
	i = 0;
	while (str[i])
	{
		if (is_in_charset(str[i], set))
			append_char_to_str(&tmp, ch);
		append_char_to_str(&tmp, str[i]);
		i++;
	}
	return (tmp);
}
