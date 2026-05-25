/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:39:16 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:39:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include "libft.h"
#include "utils.h"

int	custom_atoi(const char *str, int *out) {
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (1);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (1);
		result = result * 10 + (*str - '0');
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && result > -(long)INT_MIN))
			return (1);
		str++;
	}
	*out = (int)(result * sign);
	return (0);
}

char	*join_three(const char *a, const char *b, const char *c) {
	char	*tmp;
	char	*out;

	tmp = ft_strjoin(a, b);
	if (!tmp)
		return (NULL);
	out = ft_strjoin(tmp, c);
	free(tmp);
	return (out);
}

char	*make_coord_key(int x, int y) {
	char	*x_str;
	char	*y_str;
	char	*key;

	x_str = ft_itoa(x);
	if (!x_str)
		return (NULL);
	y_str = ft_itoa(y);
	if (!y_str)
	{
		free(x_str);
		return (NULL);
	}
	key = join_three(x_str, ":", y_str);
	free(x_str);
	free(y_str);
	return (key);
}

char	*make_link_key(const char *a, const char *b) {
	if (ft_strcmp(a, b) < 0)
		return (join_three(a, "-", b));
	return (join_three(b, "-", a));
}