/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:23:25 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/25 11:23:49 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	ft_print_char(t_data *data, char c)
{
	data->flag[DIESE] = 0;
	data->flag[SPACE] = 0;
	data->flag[PLUS] = 0;
	data->flag[PRECISION] = 0;
	data->flag[ZERO] = 0;
	if (data->flag[MINUS] == 1)
		data->n_tail = data->field - 1;
	else
		data->n_head = data->field - 1;
	ft_print_head(data);
	write(1, &c, 1);
	data->lenght++;
	ft_print_tail(data);
	return (0);
}
