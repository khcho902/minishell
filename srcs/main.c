/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/10/27 01:16:05 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH 256

int		main(void)
{
	char	*input;
	char	buf[MAX_PATH];
	char	**split;
	int		i;

	while (1)
	{
		ft_putstr_fd("~/minishell ", STDIN_FILENO);
		if (get_next_line(STDIN_FILENO, &input) > -1)
		{
			i = 0;
			split = ft_split(input, ' ');
			while (split[i])
			{
				printf("[%d]: [%s]\n", i, split[i]);
				i++;
			}
			if (ft_strncmp(input, "pwd", 3) == 0)
				printf("pwd[%s]\n", getcwd(buf, MAX_PATH));
			else if (ft_strncmp(input, "cd", 2) == 0)
			{
				chdir("/Users/jiseo/work");
				printf("change directory[%s]\n", getcwd(buf, MAX_PATH));
			}
			else if (ft_strncmp(input, "exit", 4) == 0)
				exit(0);
			else if (ft_strncmp(input, "echo", 4) == 0)
				printf("input:[%s]\n", input);
			printf("split %p input %p\n", split, input);
			ft_double_free(split);
			free(input);
			printf("split %p input %p\n", split, input);
		}
	}
}
