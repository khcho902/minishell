/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_char_to_str.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 19:38:37 by kycho             #+#    #+#             */
/*   Updated: 2020/12/20 19:39:02 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char_to_str(char **str, char c)
{
	char	*tmp;
	size_t	str_len;

	str_len = ft_strlen(*str);
	if (!(tmp = malloc(sizeof(char) * (str_len + 2))))
		exit_print_err(strerror(errno));
	ft_strlcpy(tmp, *str, str_len + 1);
	tmp[str_len] = c;
	tmp[str_len + 1] = '\0';
	free(*str);
	*str = tmp;
}
