/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:38:39 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/25 14:27:54 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>

int	ft_write(t_data *data, char *str, int n)
{
	int	len;

	len = ft_strlen(str);
	while (--n >= 0)
	{
		write (1, str, len);
		data->lenght += len;
	}
	return (0);
}

int	ft_calc_str(t_data *data, char **str)
{
	int	size;

	if (*str == 0)
	{
		if ((data->precision >= 6 && data->flag[PRECISION])
			|| !data->flag[PRECISION])
		{
			*str = "(null)";
			data->precision = 6;
		}
		else
			*str = "";
	}
	if (!data->flag[PRECISION])
		size = ft_strlen(*str);
	else
		size = ft_min(ft_strlen(*str), data->precision);
	if (size < data->field)
	{
		if (data->flag[MINUS] == 1)
			data->n_tail = data->field - size;
		else
			data->n_head = data->field - size;
	}
	return (size);
}

int	ft_calc_int(t_data *data, int size)
{
	int	big_size;

	if (data->flag[MINUS] == 1 || data->flag[PRECISION] == 1)
		data->flag[ZERO] = 0;
	if (data->flag[PRECISION] == 1 && size >= data->precision)
		data->flag[PRECISION] = 0;
	else
		data->n_zero = data->precision - size;
	if (data->n_zero < 0)
		data->n_zero = 0;
	big_size = size + data->n_zero + !(data->sign == 0
			&& !(data->flag[SPACE] == 1 || data->flag[PLUS] == 1))
		+ (data->flag[DIESE] == 1) * 2;
	big_size = data->field - big_size;
	if (big_size > 0)
	{
		if (data->flag[MINUS] == 1)
			data->n_tail = big_size;
		else if (data->flag[ZERO] == 1)
			data->n_zero += big_size;
		else
			data->n_head = big_size;
	}
	return (0);
}
