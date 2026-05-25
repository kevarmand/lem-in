/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:15:10 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/13 09:30:19 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			i;
	char			*ret;
	size_t			ls;
	size_t			max;

	if (s == 0)
		return (0);
	ls = ft_strlen(s);
	if (start >= ls)
		return (ft_calloc(1, 1));
	if (len <= ls - start)
		max = len;
	else
		max = ls - start;
	ret = malloc(max + 1);
	if (ret == 0)
		return (0);
	i = 0;
	while (i < max)
	{
		ret[i] = s[start + i];
		i++;
	}
	ret[max] = 0;
	return (ret);
}
