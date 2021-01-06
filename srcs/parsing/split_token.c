/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:31:21 by kycho             #+#    #+#             */
/*   Updated: 2021/01/06 13:22:08 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_token_sub(char *input, int *i, int *len)
{
	while (input[*i + *len])
	{
		if (input[*i + *len - 1] == '\\')
		{
			(*len)++;
//			if (input[*i + *len - 1] != '\0')
//				(*len)++;
		}
		else if (input[*i + *len - 1] == '"')
		{
			while (!(input[*i + *len] == '"'))
			{
				if (input[*i + *len] == '\\')
				{
					(*len)++;
				}
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
//		else 
//			(*len)++;
		if (!input[*i + *len] || is_in_charset(input[*i + *len], METACHARACTER))
			break;
		else
			(*len)++;
	}
}

void	split_token(char *input, t_list **tokens, int i)
{
	int		len;
	char	*str;
	t_list	*lstnew;

	while (input[i] && (len = 1))
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == 0)
			break ;
		//len = 1;
		if (!is_in_charset(input[i], METACHARACTER))
			split_token_sub(input, &i, &len);
		else if (ft_strncmp(&input[i], ">>", 2) == 0 ||
				ft_strncmp(&input[i], "<<", 2) == 0)
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
