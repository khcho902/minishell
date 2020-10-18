/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 01:43:05 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/06 23:13:36 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstnew(void *content)
{
	t_list	*elm;

	if ((elm = (t_list *)malloc(sizeof(t_list) * 1)) == NULL)
		return (NULL);
	elm->content = content;
	elm->next = NULL;
	return (elm);
}
