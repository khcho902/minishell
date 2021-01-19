/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_numeric_long_str.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 13:25:35 by kycho             #+#    #+#             */
/*   Updated: 2021/01/20 01:01:17 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_numeric_long_str_sub(int num_len, int is_minus, char *num_str)
{
	if (num_len < 19)
		return (TRUE);
	if (num_len > 19)
		return (FALSE);
	if (is_minus)
		return (ft_strncmp("9223372036854775808", num_str, 19) >= 0);
	else
		return (ft_strncmp("9223372036854775807", num_str, 19) >= 0);
}

int		is_numeric_long_str(char *str)
{
	int num_len;
	int num_start_idx;
	int is_minus;

	if (is_numeric_str(str) == FALSE)
		return (FALSE);
	is_minus = 0;
	num_start_idx = 0;
	while (str[num_start_idx] && !ft_isdigit(str[num_start_idx]))
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
	return (is_numeric_long_str_sub(num_len, is_minus, str + num_start_idx));
}
