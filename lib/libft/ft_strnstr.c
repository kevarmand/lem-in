/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:53:35 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/11 15:56:00 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

static int	is_str(const char *s1, const char *s2, size_t n)
{
	while (*s1 && *s2 && n > 0)
	{
		if (*s1 != *s2)
			return (0);
		n--;
		s1++;
		s2++;
	}
	if (*s2 == 0)
		return (1);
	return (0);
}

char	*ft_strnstr(const char *big, const char *lit, size_t n)
{
	if (*lit == 0)
		return ((char *)big);
	if (n == 0)
		return (0);
	while (*big && n > 0)
	{
		if (is_str(big, lit, n))
			return ((char *)big);
		big++;
		n--;
	}
	return (0);
}
