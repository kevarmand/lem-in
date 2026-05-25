/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:11:24 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/11 16:00:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

#include <stdio.h>

static char	*ft_zero(void)
{
	char	*ret;

	ret = malloc(2);
	if (ret == 0)
		return (0);
	ret[0] = '0';
	ret[1] = 0;
	return (ret);
}

static char	*ft_endft(char *tab, int sign, int nd)
{
	if (sign == -1)
		tab[nd] = '-';
	else
		nd++;
	tab[11] = 0;
	return (ft_substr(tab, nd, 11));
}

static void	ft_init(char *tab)
{
	int	i;

	i = -1;
	while (++i < 12)
		tab[i] = 1;
}

char	*ft_itoa(int n)
{
	int				nd;
	char			tab[12];
	unsigned int	nb;
	int				sign;

	ft_init(tab);
	if (n == 0)
		return (ft_zero());
	nd = 10;
	sign = 1;
	if (n < 0)
	{
		sign = -1;
		nb = (unsigned int)(-n);
	}
	else
		nb = n;
	while (nb != 0)
	{
		tab[nd] = '0' + (nb % 10);
		nd--;
		nb = nb / 10;
	}
	return (ft_endft(tab, sign, nd));
}
