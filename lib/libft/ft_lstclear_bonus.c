/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:31:29 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/09 17:46:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	if (lst != 0 && *lst != 0)
	{
		if ((*lst)->next != 0)
			ft_lstclear(&((*lst)->next), del);
		del((*lst)->content);
		free(*lst);
		*lst = 0;
	}
}
