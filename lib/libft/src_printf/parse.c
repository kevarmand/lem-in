/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:32:47 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/23 14:07:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_switch_type(t_data *data, va_list *ap)
{
	int		test;

	test = data->type;
	if (test == CHAR)
		return (ft_print_char(data, (char)va_arg(*ap, int)));
	if (test == STR)
		return (ft_print_str(data, va_arg(*ap, char *)));
	if (test == POINT)
		return (ft_print_pointer(data, va_arg(*ap, unsigned long)));
	if (test == INT || test == INT + 1)
		return (ft_print_int(data, va_arg(*ap, int)));
	if (test == UNSIGNED)
		return (ft_print_uns(data, va_arg(*ap, unsigned int)));
	if (test == HEX)
		return (ft_print_hex(data, va_arg(*ap, unsigned int)));
	if (test == HEXA)
		return (ft_print_hex(data, va_arg(*ap, unsigned int)));
	ft_write(data, "%", 1);
	return (0);
}

static int	ft_parse_type(char **str, t_data *data)
{
	int	i;

	i = -1;
	while (++i < NB_TYPE)
	{
		if (TTYPE[i] == **str)
		{
			(*str)++;
			if (i == 5)
				data->type = 4;
			else
				data->type = i;
			return (1);
		}
	}
	return (0);
}

static int	ft_parse_s(char **str, t_data *data)
{
	(*str)++;
	while (ft_is_flag(**str, data))
		(*str)++;
	ft_parse_field(str, data);
	return (ft_parse_type(str, data));
}

int	ft_parse(char *str, t_data *data, va_list *ap)
{
	int		i;

	while (*str)
	{
		i = 0;
		while (str[i] && str[i] != '%')
		{
			i++;
		}
		if (i != 0)
		{
			write(1, str, i);
			str += i;
			data->lenght += i;
		}
		if (*str == '%')
		{
			ft_init(data);
			if (ft_parse_s(&str, data) == 0)
				return (-1);
			ft_switch_type(data, ap);
		}
	}
	return (0);
}
