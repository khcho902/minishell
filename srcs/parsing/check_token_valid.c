/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token_valid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:32:52 by kycho             #+#    #+#             */
/*   Updated: 2021/01/18 01:19:08 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_token_valid_sub(char *program_name, t_list *now, int *before_type)
{
	if (((char*)now->content)[0] == '|')
	{
		if (*before_type != TOKEN_WORD)
			return (print_syntax_err(program_name, now->content, FALSE));
		*before_type = TOKEN_PIPE;
	}
	else if (((char *)now->content)[0] == ';')
	{
		if (*before_type != TOKEN_WORD)
			return (print_syntax_err(program_name, now->content, FALSE));
		*before_type = TOKEN_SEMICOLON;
	}
	else if (((char *)now->content)[0] == '>' ||
			((char *)now->content)[0] == '<')
	{
		if (*before_type == TOKEN_REDIRECTION)
			return (print_syntax_err(program_name, now->content, FALSE));
		*before_type = TOKEN_REDIRECTION;
	}
	else
		*before_type = TOKEN_WORD;
	return (SUCCESS);
}

int	check_token_valid(char *program_name, t_list *now)
{
	int		before_type;

	before_type = TOKEN_NONE;
	while (now)
	{
		if (check_token_valid_sub(program_name, now, &before_type) == ERROR)
			return (ERROR);
		now = now->next;
	}
	if (before_type == TOKEN_REDIRECTION)
		return (print_syntax_err(program_name, "newline", FALSE));
	if (before_type == TOKEN_PIPE)
		return (ERROR);
	return (SUCCESS);
}
