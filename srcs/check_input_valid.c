/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_valid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 19:38:54 by kycho             #+#    #+#             */
/*   Updated: 2021/01/26 19:39:23 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_quotes_valid(char *program_name, char *input)
{
	int in_dquotes;
	int i;

	in_dquotes = FALSE;
	i = -1;
	while (input[++i])
	{
		if (input[i] == '\\')
			i++;
		else if (in_dquotes == FALSE && input[i] == '\'')
		{
			i++;
			while (input[i] != '\'' && input[i] != '\0')
				i++;
			if (input[i] != '\'')
				return (print_syntax_err(program_name, "'", TRUE));
		}
		else if (input[i] == '"')
			in_dquotes ^= TRUE;
		if (input[i] == 0)
			break ;
	}
	if (in_dquotes == TRUE)
		return (print_syntax_err(program_name, "\"", TRUE));
	return (SUCCESS);
}

int		check_token_valid_sub(char *program_name, t_list *now, int *before_type)
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

int		check_token_valid(char *program_name, t_list *now)
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

int		check_input_valid(t_msh *msh, char *input)
{
	t_list	*tokens;

	if (check_quotes_valid(msh->program_name, input) == ERROR)
	{
		g_exit_status = 258;
		return (ERROR);
	}
	split_token(input, &tokens, METACHARACTER);
	if (check_token_valid(msh->program_name, tokens) == ERROR)
	{
		g_exit_status = 258;
		ft_lstclear(&(tokens), free);
		return (ERROR);
	}
	ft_lstclear(&(tokens), free);
	return (SUCCESS);
}
