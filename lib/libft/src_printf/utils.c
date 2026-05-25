/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:57:44 by kearmand          #+#    #+#             */
/*   Updated: 2024/12/13 10:54:44 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	ft_init(t_data *data)
{
	int	i;

	i = -1;
	data->type = 0;
	while (++i < NBFLAG + 1)
		data->flag[i] = 0;
	data->field = 0;
	data->precision = 0;
	data->size = 0;
	data->sign = 0;
	data->n_head = 0;
	data->n_zero = 0;
	data->n_tail = 0;
}

void	ft_skip(char **str)
{
	while (**str && (**str >= '0' && **str <= '9'))
		(*str)++;
}

int	ft_atoi_pf(char **str)
{
	int	t;

	if (**str == '-')
	{
		(*str)++;
		ft_skip(str);
		return (0);
	}
	t = 0;
	while (**str && (**str >= '0' && **str <= '9'))
	{
		if ((t == 214748364 && **str >= '7') || t > 2147483640)
		{
			ft_skip(str);
			return (2147483647);
		}
		t = (10 * t) + (**str - '0');
		(*str)++;
	}
	return (t);
}

int	ft_putnbr_base(t_data *data, char *tab, long n, char *base)
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
