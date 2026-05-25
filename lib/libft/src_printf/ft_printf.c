/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:17:22 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/27 11:52:52 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	t_data	data;

	if (format == 0)
		return (-1);
	va_start(ap, format);
	ft_init(&data);
	data.lenght = 0;
	if (-1 == ft_parse((char *)format, &data, &ap))
		return (-1);
	va_end(ap);
	return (data.lenght);
}
