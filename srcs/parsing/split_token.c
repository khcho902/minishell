/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:31:21 by kycho             #+#    #+#             */
/*   Updated: 2021/01/09 15:50:11 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_token_sub(char *input, char *separator, int *i, int *len)
{
	while (input[*i + *len])
	{
		if (input[*i + *len - 1] == '\\')
			(*len)++;
		else if (input[*i + *len - 1] == '"')
		{
			while (!(input[*i + *len] == '"'))
			{
				if (input[*i + *len] == '\\')
					(*len)++;
				(*len)++;
			}
			(*len)++;
		}
		else if (input[*i + *len - 1] == '\'')
		{
			while (!(input[*i + *len] == '\''))
				(*len)++;
			(*len)++;
		}
		if (!input[*i + *len] || is_in_charset(input[*i + *len], separator))
			break ;
		else
			(*len)++;
	}
}

void	split_token(char *input, t_list **tokens, char *separator)
{
	int		i;
	int		len;
	char	*str;
	t_list	*lstnew;

	i = 0;
	*tokens = NULL;
	while (input[i] && (len = 1))
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == 0)
			break ;
		if (!is_in_charset(input[i], separator))
			split_token_sub(input, separator, &i, &len);
		else if (ft_strncmp(&input[i], ">>", 2) == 0 ||
				ft_strncmp(&input[i], "<<", 2) == 0)
			len++;
		if (!(str = ft_substr(input, i, len)))
			exit_print_err(strerror(errno));
		if (!(lstnew = ft_lstnew(str)))
			exit_print_err(strerror(errno));
		ft_lstadd_back(tokens, lstnew);
		i += len;
	}
}
