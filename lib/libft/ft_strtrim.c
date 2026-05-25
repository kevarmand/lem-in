/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:38:20 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/13 09:34:00 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		else
			set++;
	}
	return (0);
}

char	*ft_strtrim(const char *str, const char *set)
{
	int		end;

	if (!str || !set)
		return (0);
	while (*str && is_in_set(*str, set))
		str++;
	end = ft_strlen(str);
	while (end > 0 && is_in_set(str[end - 1], set))
		end--;
	return (ft_substr(str, 0, end));
}
