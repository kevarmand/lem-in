/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:29:55 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/06 10:38:12 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h> 

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*temp;
	unsigned char	m;
	size_t			i;

	temp = (unsigned char *)s;
	i = 0;
	m = (unsigned char)c;
	while (i < n)
	{
		temp[i] = m;
		i++;
	}
	return (s);
}
