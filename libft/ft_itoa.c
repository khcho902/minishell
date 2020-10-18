/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 07:20:45 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/04 03:55:16 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char	result[12];
	int		int_max_digit;
	int		i;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	int_max_digit = 1000000000;
	i = 0;
	if (n < 0)
	{
		result[i++] = '-';
		n *= -1;
	}
	while ((n / int_max_digit) == 0)
		int_max_digit /= 10;
	while (int_max_digit != 0)
	{
		result[i++] = ((n / int_max_digit) % 10) + '0';
		int_max_digit /= 10;
	}
	result[i] = '\0';
	return (ft_strdup(result));
}
