/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:31:21 by kycho             #+#    #+#             */
/*   Updated: 2020/12/19 20:21:40 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_token_sub(char *input, int *i, int *len)
{
	while (input[*i + *len])
	{
		if (input[*i + *len - 1] == '"'
				&& (*i + *len - 1 == 0 || input[*i + *len - 2] != '\\'))
		{
			while (!(input[*i + *len] == '"' && input[*i + *len - 1] != '\\')
					&& input[*i + *len] != 0)
				(*len)++;
			(*len)++;
		}
		if (input[*i + *len - 1] == '\''
				&& (*i + *len - 1 == 0 || input[*i + *len - 2] != '\\'))
		{
			while (!(input[*i + *len] == '\'') && input[*i + *len] != 0)
				(*len)++;
			(*len)++;
		}
		if (is_in_charset(input[*i + *len], METACHARACTER)
				&& input[*i + *len - 1] != '\\')
		{
			break ;
		}
		(*len)++;
	}
}

void	split_token(char *input, t_list **tokens, int i)
{
	int		len;
	char	*str;
	t_list	*lstnew;

	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == 0)
			break ;
		len = 1;
		if (!is_in_charset(input[i], METACHARACTER))
			split_token_sub(input, &i, &len);
		else if (ft_strncmp(&input[i], ">>", 2) == 0)
			len++;
		if (!(str = ft_substr(input, i, len)))
			exit_print_err(strerror(errno));
		if (!(lstnew = ft_lstnew(str)))
			exit_print_err(strerror(errno));
		if (*tokens == NULL)
			*tokens = lstnew;
		else
			ft_lstadd_back(tokens, lstnew);
		i += len;
	}
}
