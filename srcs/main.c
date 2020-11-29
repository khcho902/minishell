/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/29 16:58:10 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH 256

#define FALSE 0
#define TRUE 1
#define METACHARACTER " \t\n|;<>"

int		is_in_charset(char c, char *str)
{
	int idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == c)
			return (TRUE);
		idx++;
	}
	return (FALSE);
}

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
			while (!(input[*i + *len] == '\'' && input[*i + *len - 1] != '\\')
					&& input[*i + *len] != 0)
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

void	split_token(char *input, t_list **tokens)
{
	int i;
	int len;

	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == 0)
			break ;
		len = 1;
		if (is_in_charset(input[i], METACHARACTER))
		{
			if (input[i] == '>' && input[i + 1] == '>')
				len++;
		}
		else
			split_token_sub(input, &i, &len);
		if (*tokens == NULL)
			*tokens = ft_lstnew(ft_substr(input, i, len));
		else
			ft_lstadd_back(tokens, ft_lstnew(ft_substr(input, i, len)));
		i += len;
	}
}

int		check_token_valid(t_list **tokens)
{
	t_list	*now;
	int		before_type;

	before_type = 0;
	now = *tokens;
	while(now)
	{
		if (((char*)now->content)[0] == '|' || ((char *)now->content)[0] == ';')
		{
			if (before_type != 1)
			{
				printf("-bash: syntax error near unexpected token `%s'\n", now->content);
				return (-1);
			}
			before_type = 2;
		}
		else if (((char *)now->content)[0] == '>' || ((char *)now->content)[0] == '<')
		{
			if (before_type == 3)
			{
				printf("-bash: syntax error near unexpected token `%s'\n", now->content);
				return (-1);
			}
			before_type = 3;
		}
		else 
		{
			before_type = 1;
		}
		now = now->next;
	}

	if (before_type == 3)
	{
		printf("-bash: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	return (1);
}

int		main(void)
{
	char	*input;
	int		res;
	t_list	*tokens;

	res = 1;
	while (res)
	{
		ft_putstr_fd("minishell$ ", STDOUT_FILENO);
		res = get_next_line(STDIN_FILENO, &input);
		if (res == -1)
		{
			exit(0);  // TODO : 에러발생 출력
		}
		else 
		{
			printf("input : %s\n", input);

			tokens = NULL;
			split_token(input, &tokens);

			if (check_token_valid(&tokens) == -1)
			{
				printf("error\n");
			}
			else
			{
				printf("%d\n", ft_lstsize(tokens));
				t_list *now = tokens;
				while(now)
				{
					printf("|%s|\n", now->content);
					now = now->next;
				}
			}
			ft_lstclear(&tokens, free);
			free(input);
		}
	}
	return (0);
}
