/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 23:11:23 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/13 06:22:28 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		show_prompt(t_msh *msh)
{
	char		*temp;
	const char	*home = "/home/jiseo";
	int			home_length;

	msh->wd = getcwd(NULL, 0);
	home_length = ft_strlen(home);
	if (ft_strncmp(msh->wd, home, home_length) == 0)
		temp = ft_strjoin3("~", &msh->wd[home_length], "$ ");
	else
		temp = ft_strjoin(msh->wd, "$ ");
	ft_putstr_fd(temp, STDOUT);
	free(temp);
}
