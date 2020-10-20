/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/10/20 16:40:21 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_PATH 256

int		main(void)
{
	char	*input;
	char	buf[MAX_PATH];

	while (get_next_line(STDIN_FILENO, &input) > -1)
	{
		if (ft_strncmp(input, "pwd", 3) == 0)
			printf("pwd[%s]\n", getcwd(buf, MAX_PATH));
		else if (ft_strncmp(input, "cd", 2) == 0)
		{
			chdir("/Users/jiseo/work");
			printf("change directory[%s]\n", getcwd(buf, MAX_PATH));
		}
		else if (ft_strncmp(input, "exit", 4) == 0)
			exit(0);
		printf("input:[%s]\n", input);
		free(input);
	}
}
