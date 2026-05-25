/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:40:23 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/25 14:44:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	ft_putnbr_base_u(t_data *data, char *tab, unsigned long n, char *base)
{
	int	size;
	int	b;

	b = ft_strlen(base);
	if (n < 0)
	{
		n = -n;
		data->sign = -1;
	}
	if (n == 0)
	{
		tab[TABSIZE - 1] = '0';
		return (1);
	}
	size = 0;
	while (n != 0)
	{
		tab[TABSIZE - 1 - size] = base[n % b];
		size++;
		n = n / b;
	}
	return (size);
}
