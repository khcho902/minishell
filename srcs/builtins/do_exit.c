/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:35:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 15:37:09 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		do_exit(t_msh *msh)
{
	free_msh_member(msh);
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
