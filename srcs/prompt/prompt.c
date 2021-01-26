/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 23:11:23 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/19 11:58:55 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		show_logo(void)
{
	ft_putstr_fd(COLOR_BLUE, STDOUT);
	ft_putstr_fd("        :::      ::::::::\n", STDOUT);
	ft_putstr_fd("      :+:      :+:    :+:\n", STDOUT);
	ft_putstr_fd("    +:+ +:+         +:+  \n", STDOUT);
	ft_putstr_fd("  +#+  +:+       +#+	   \n", STDOUT);
	ft_putstr_fd("+#+#+#+#+#+   +#+        \n", STDOUT);
	ft_putstr_fd("     #+#    #+#          \n", STDOUT);
	ft_putstr_fd("    ###   ########.kr    \n", STDOUT);
	ft_putstr_fd(COLOR_BR_GREEN, STDOUT);
	ft_putstr_fd("by. kycho, jiseo\n", STDOUT);
}

void		show_prompt(void)
{
	ft_putstr_fd(COLOR_BR_BLUE, STDOUT);
	ft_putstr_fd("minishell", STDOUT);
	ft_putstr_fd(COLOR_WHITE, STDOUT);
	ft_putstr_fd("$ ", STDOUT);
}
