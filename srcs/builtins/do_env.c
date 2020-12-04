/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:00:35 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/05 05:04:11 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_env(t_list *env_list, int fd)
{
	printf("--- env start ---[size:%d]\n", ft_lstsize(env_list));
	print_env(env_list, fd);
	printf("--- env end ---\n");
}
