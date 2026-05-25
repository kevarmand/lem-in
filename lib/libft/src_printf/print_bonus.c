/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:43:19 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/25 13:41:57 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static void	ft_print_sp(t_data *data)
{
	if (data->type == INT)
	{
		if (data->sign == -1)
		{
			ft_write(data, "-", 1);
			return ;
		}
	}
	if (data->type == INT || data->type == POINT)
	{
		if (data->flag[3] == 1)
			ft_write(data, "+", 1);
		else if (data->flag[1] == 1)
			ft_write(data, " ", 1);
	}
}

static void	ft_print_diese(t_data *data)
{
	if (data->flag[0] == 1)
	{
		if (data->type == HEX || data->type == POINT)
			ft_write(data, "0x", 1);
		if (data->type == HEXA)
			ft_write(data, "0X", 1);
	}
}

void	ft_print_head(t_data *data)
{
	ft_write(data, " ", data->n_head);
	ft_print_sp(data);
	ft_print_diese(data);
	ft_write(data, "0", data->n_zero);
}

void	ft_print_tail(t_data *data)
{
	ft_write(data, " ", data->n_tail);
}
