/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 02:05:08 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(int ac, char **av, char **env)
{
	t_msh	msh;
	int		res;

	init_main(&msh, env);
	init_msh(av[0], &msh, env);
	res = ac;
	while (res)
	{
		show_prompt(&msh);
		res = get_next_line(STDIN, &(msh.input));
		main_loop(&msh);
		free(msh.input);
	}
	return (0);
}
