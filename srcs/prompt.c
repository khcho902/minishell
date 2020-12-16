/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 23:11:23 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 03:01:06 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		show_logo()
{
	ft_putstr_fd(COLOR_BLUE, STDOUT);
	ft_putstr_fd("        :::      ::::::::\n", STDOUT);
	ft_putstr_fd("      :+:      :+:    :+:\n", STDOUT);
	ft_putstr_fd("    +:+ +:+         +:+  \n", STDOUT);
	ft_putstr_fd("  +#+  +:+       +#+	   \n", STDOUT);
	ft_putstr_fd("+#+#+#+#+#+   +#+        \n", STDOUT);
	ft_putstr_fd("     #+#    #+#          \n", STDOUT);
	ft_putstr_fd("    ###   ########.kr    \n", STDOUT);
	ft_putstr_fd(COLOR_WHITE, STDOUT);
	ft_putstr_fd("by. kycho, jiseo\n", STDOUT);
}

void		show_prompt(t_msh *msh)
{
	char		*temp;
	const char	*home = "/home/jiseo";
	int			home_length;

	msh->wd = getcwd(NULL, 0);
	home_length = ft_strlen(home);
	if (ft_strncmp(msh->wd, home, home_length) == 0)
		temp = ft_strjoin("~", &msh->wd[home_length]);
	else
		temp = ft_strdup(msh->wd);
	ft_putstr_fd(COLOR_BR_BLUE, STDOUT);
	ft_putstr_fd(temp, STDOUT);
	ft_putstr_fd(COLOR_WHITE, STDOUT);
	ft_putstr_fd("$ ", STDOUT);
	free(temp);
}
