/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/28 21:58:23 by kycho            ###   ########.fr       */
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

void	split_token(char *input, char **words)
{
	int i;
	int word_cnt;
	int len;

	i = 0;
	word_cnt = 0;
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
		words[word_cnt] = ft_substr(input, i, len);
		word_cnt++;
		i += len;
	}
	words[word_cnt] = NULL;
}

int		main(void)
{
	char	*input;
	int		res;
	char	*words[255];


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

			split_token(input, words);
			
			for(int j = 0; words[j] != NULL ; j++){
				printf("|%s|\n", words[j]);
				int cnt = 0;
				for(int k = 0; k < (int)ft_strlen(words[j]); k++){
					if (words[j][k] == '\t')
						cnt++;
				}
				//printf("%d\n",cnt);
			}

			for(int j = 0; words[j] != NULL; j++){
				free(words[j]);
			}


			free(input);
		}
	}
	return (0);
}
