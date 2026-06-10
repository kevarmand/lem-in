/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:39:16 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 11:31:02 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include "libft.h"
#include "utils.h"

int	custom_atoi(const char *str, int *out) {
	long	result;
	long	limit;
	int		sign;
	int		digit;

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
	if (sign == -1)
		limit = -(long)INT_MIN;
	else
		limit = INT_MAX;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (1);
		digit = *str - '0';
		if (result > (limit - digit) / 10)
			return (1);
		result = result * 10 + digit;
		str++;
	}
	if (sign == -1)
		*out = (int)-result;
	else
		*out = (int)result;
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