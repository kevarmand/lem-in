/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:40:23 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/01 13:11:20 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	ft_putnbr_base_u(t_data *data, char *tab, unsigned long n, char *base)
{
	int	size;
	int	b;
	(void)data;

	b = ft_strlen(base);
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
