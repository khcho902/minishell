/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 23:11:23 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/20 23:11:36 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		show_prompt(char *str)
{
	ft_putstr_fd(str, STDIN_FILENO);
	ft_putchar_fd(' ', STDIN_FILENO);
}
