/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 04:46:18 by jiseo             #+#    #+#             */
/*   Updated: 2020/04/06 23:02:13 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*next_new;
	t_list	*next_lst;

	if ((new_list = ft_lstnew((*f)(lst->content))) == NULL)
		return (NULL);
	next_new = new_list;
	next_lst = lst->next;
	while (next_lst != NULL)
	{
		if ((next_new->next = ft_lstnew((*f)(next_lst->content))) == NULL)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		next_new = next_new->next;
		next_lst = next_lst->next;
	}
	return (new_list);
}
