/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:17:26 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/11 15:19:31 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	is_wspace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	unsigned int	t;
	unsigned int	sign;

	sign = 1;
	while (*str && is_wspace(*str))
		str++;
	if (*str && (*str == '-' || *str == '+'))
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	t = 0;
	while (*str && (*str >= '0' && *str <= '9'))
	{
		t = (10 * t) + (*str - '0');
		str++;
	}
	return ((int)(t * sign));
}
