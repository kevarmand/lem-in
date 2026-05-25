/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:08:10 by kearmand          #+#    #+#             */
/*   Updated: 2024/12/13 10:42:38 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_is_flag(char str, t_data *data)
{
	int	i;

	i = -1;
	while (++i < NBFLAG)
	{
		if (TFLAG[i] == str)
		{
			data->flag[i] = 1;
			return (1);
		}
	}
	return (0);
}

int	ft_parse_field(char **str, t_data *data)
{
	if (**str != '.')
		data->field = ft_atoi_pf(str);
	if (**str == '.')
	{
		(*str)++;
		data->precision = ft_atoi_pf(str);
		data->flag[NBFLAG] = 1;
	}
	return (0);
}
