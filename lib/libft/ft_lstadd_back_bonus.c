/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:25:29 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/05 18:10:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (lst == 0)
		return ;
	temp = *lst;
	if (temp == 0)
	{
		*lst = new;
		return ;
	}
	while (temp->next != 0)
		temp = temp->next;
	temp->next = new;
}
