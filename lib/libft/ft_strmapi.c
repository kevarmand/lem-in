/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:44:01 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/13 09:36:12 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdlib.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	unsigned int	len;
	char			*ret;
	unsigned int	index;

	if (!s || !f)
		return (0);
	index = -1;
	len = ft_strlen(s);
	ret = malloc(len + 1);
	if (ret == 0)
		return (0);
	while (++index < len)
		ret[index] = f(index, s[index]);
	ret[len] = 0;
	return (ret);
}
