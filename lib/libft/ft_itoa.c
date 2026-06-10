/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:11:24 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 11:35:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ft_int_len(long nb) {
	int	len;

	len = 1;
	if (nb < 0)
	{
		len++;
		nb = -nb;
	}
	while (nb >= 10)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n) {
	char	*out;
	long	nb;
	int		len;

	nb = (long)n;
	len = ft_int_len(nb);
	out = malloc(sizeof(*out) * (len + 1));
	if (!out)
		return (NULL);
	out[len] = '\0';
	if (nb == 0)
		out[0] = '0';
	if (nb < 0)
	{
		out[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		len--;
		out[len] = '0' + (nb % 10);
		nb /= 10;
	}
	return (out);
}
