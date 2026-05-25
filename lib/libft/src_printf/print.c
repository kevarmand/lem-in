/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:49:28 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/25 14:46:40 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	ft_print_str(t_data *data, char *str)
{
	int	size;

	data->flag[DIESE] = 0;
	data->flag[SPACE] = 0;
	data->flag[ZERO] = 0;
	data->flag[PLUS] = 0;
	size = ft_calc_str(data, &str);
	ft_print_head(data);
	if (size != 0)
		write(1, str, size);
	data->lenght += size;
	ft_print_tail(data);
	return (0);
}

int	ft_print_int(t_data *data, int n)
{
	char	tab[TABSIZE + 1];
	int		size;

	tab[TABSIZE] = 0;
	size = ft_putnbr_base(data, tab, n, "0123456789");
	if (data->flag[PRECISION] == 1 && data->precision == 0 && n == 0)
		size = 0;
	data->flag[DIESE] = 0;
	ft_calc_int(data, size);
	data->size = size;
	ft_print_head(data);
	ft_write(data, tab + (TABSIZE - size), 1);
	ft_print_tail(data);
	return (0);
}

int	ft_print_hex(t_data *data, unsigned int n)
{
	char	tab[TABSIZE + 1];
	int		size;

	tab[TABSIZE] = 0;
	if (data->type == HEX)
		size = ft_putnbr_base(data, tab, n, "0123456789abcdef");
	else
		size = ft_putnbr_base(data, tab, n, "0123456789ABCDEF");
	if (data->flag[PRECISION] == 1 && data->precision == 0 && n == 0)
		size = 0;
	if (n == 0)
		data->flag[DIESE] = 0;
	data->flag[SPACE] = 0;
	data->flag[PLUS] = 0;
	ft_calc_int(data, size);
	data->size = size;
	ft_print_head(data);
	ft_write(data, tab + (TABSIZE - size), 1);
	ft_print_tail(data);
	return (0);
}

int	ft_print_uns(t_data *data, unsigned int n)
{
	char	tab[TABSIZE + 1];
	int		size;

	tab[TABSIZE] = 0;
	size = ft_putnbr_base(data, tab, n, "0123456789");
	if (data->flag[PRECISION] == 1 && data->precision == 0 && n == 0)
		size = 0;
	data->flag[DIESE] = 0;
	data->flag[SPACE] = 0;
	data->flag[PLUS] = 0;
	ft_calc_int(data, size);
	data->size = size;
	ft_print_head(data);
	ft_write(data, tab + (TABSIZE - size), 1);
	ft_print_tail(data);
	return (0);
}

int	ft_print_pointer(t_data *data, unsigned long n)
{
	char	tab[TABSIZE + 1];
	int		size;

	tab[TABSIZE] = 0;
	size = ft_putnbr_base_u(data, tab, n, "0123456789abcdef");
	if (n == 0)
	{
		data->precision = 5;
		ft_print_str(data, "(nil)");
		return (0);
	}
	data->flag[DIESE] = 1;
	ft_calc_int(data, size);
	data->size = size;
	ft_print_head(data);
	ft_write(data, tab + (TABSIZE - size), 1);
	ft_print_tail(data);
	return (0);
}
