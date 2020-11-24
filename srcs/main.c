/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/25 02:16:43 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH 256


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
			// TODO : 에러발생 출력
			exit(0);
		}
		else 
		{
			printf("input : %s\n", input);
			
			int word_start = 0; 
			int word_cnt = 0;

			while (input[word_start])
			{
				while(input[word_start] == ' ')
					word_start++;
				if (input[word_start] == 0)
					break;

				int word_len = 1;
				int only_num = 1;

				while(input[word_start + word_len])
				{
					if (!ft_isdigit(input[word_start + word_len - 1]))
						only_num = 0;

					if (input[word_start + word_len - 1] == '"'
							&& ( word_start + word_len - 1 == 0 || input[word_start + word_len - 2] != '\\'))
					{
						while(!(input[word_start + word_len] == '"' && input[word_start + word_len - 1] != '\\') 
								&& input[word_start + word_len] != 0)
							word_len++;
						word_len++;
					}
					
					if (input[word_start + word_len - 1] == '\''
							&& ( word_start + word_len - 1 == 0 || input[word_start + word_len - 2] != '\\'))
					{
						while(!(input[word_start + word_len] == '\'' && input[word_start + word_len - 1] != '\\') 
								&& input[word_start + word_len] != 0)
							word_len++;
						word_len++;
					}
					/*
					if (input[word_start + word_len - 1] == '\'')
					{
						while(input[word_start + word_len] != '\'' && input[word_start + word_len] != 0)
							word_len++;
						word_len++;
					}
					*/
			

					// --------------------------------------------------------
					if (input[word_start] == '>' && input[word_start + 1] == '>')
					{
						word_len++;
						break;
					}

					if (input[word_start] == '>')
						break;

					if (only_num == 0 && input[word_start + word_len] == '>' && input[word_start + word_len - 1] != '\\')
						break;

					if (only_num == 1 && input[word_start + word_len] == '>' && input[word_start + word_len + 1] == '>')
					{
						word_len += 2;
						break;
					}

					if (only_num == 1 && input[word_start + word_len] == '>')
					{
						word_len++;
						break;
					}

					// --------------------------------------------------------

					if (input[word_start] == '<' || (input[word_start + word_len] == '<' && input[word_start + word_len - 1] != '\\'))
						break;

					if (input[word_start] == ';' || (input[word_start + word_len] == ';' && input[word_start + word_len - 1] != '\\'))
						break;

					if (input[word_start] == '|' || (input[word_start + word_len] == '|' && input[word_start + word_len - 1] != '\\'))
						break;

					if (input[word_start + word_len] == ' ')
						break;

					word_len++;
				}

				
				words[word_cnt] = ft_substr(input, word_start, word_len);
				word_cnt++;

				word_start += word_len;
			}

	
			for(int j = 0; j < word_cnt; j++){
				printf("|%s|\n", words[j]);
			}

			for(int j = 0;j < word_cnt; j++){
				free(words[j]);
			}


			free(input);
		}
	}
	return (0);
}
