/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:54:45 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/11 14:57:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*cdest;
	unsigned char	*csrc;

	cdest = (unsigned char *)dest;
	csrc = (unsigned char *)src;
	if (n == 0)
		return (dest);
	i = -1;
	if (dest == src)
		return (dest);
	if (cdest < csrc)
		while (++i < n)
			cdest[i] = csrc[i];
	else
	{
		while (--n > 0)
			cdest[n] = csrc[n];
		cdest[0] = csrc[0];
	}
	return (dest);
}
