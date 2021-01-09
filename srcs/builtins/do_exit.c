/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:35:33 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/10 00:28:33 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_exit_argument(char *str, int is_minus)
{
	int num_len;
	int num_start_idx;

	if (is_numeric_str(str) == FALSE)
		return (FALSE);
	num_start_idx = 0;
	while (!ft_isdigit(str[num_start_idx]))
	{
		if (str[num_start_idx] == '-')
			is_minus = 1;
		num_start_idx++;
	}
	while (str[num_start_idx] == '0')
		num_start_idx++;
	num_len = 0;
	while (ft_isdigit(str[num_start_idx + num_len]))
		num_len++;
	if (num_len < 19)
		return (TRUE);
	if (num_len > 19)
		return (FALSE);
	if (is_minus)
		return (ft_strcmp("9223372036854775808", str + num_start_idx) >= 0);
	else
		return (ft_strcmp("9223372036854775807", str + num_start_idx) >= 0);
}

void	do_exit(t_msh *msh, t_cmd *cmd)
{
	char *program_name;
	char *tmp;

	program_name = msh->program_name;
	if (cmd->length == 1)
		exit(msh->exit_status);
	if (check_exit_argument(cmd->args[1], 0) == FALSE)
	{
		if (!(tmp = ft_strjoin3(cmd->args[0], ": ", cmd->args[1])))
			exit_print_err(strerror(errno));
		print_execute_err(program_name, tmp, "numeric argument required");
		msh->exit_status = 255;
		exit(msh->exit_status & 255);
	}
	if (cmd->length == 2)
		msh->exit_status = ft_atoi(cmd->args[1]);
	else
	{
		print_execute_err(program_name, cmd->args[0], "too many arguments");
		msh->exit_status = 1;
	}
	exit(msh->exit_status);
}
