/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:05:59 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/11 16:44:39 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	lsrc;

	i = 0;
	lsrc = ft_strlen(src);
	if (size == 0)
		return (lsrc);
	while (*dest && i < size)
	{
		dest++;
		i++;
	}
	if (size == 0 || i >= size)
		return (size + lsrc);
	lsrc += i;
	while (*src && i < size - 1)
	{
		*dest = *src;
		src++;
		dest++;
		i++;
	}
	*dest = 0;
	return (lsrc);
}
