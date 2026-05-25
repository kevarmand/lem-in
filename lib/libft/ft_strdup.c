/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:51:35 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/07 15:20:09 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *str)
{
	int		len;
	char	*ret;

	len = 1 + ft_strlen(str);
	ret = malloc(len * sizeof(char));
	if (ret == 0)
		return (0);
	len--;
	while (len >= 0)
	{
		ret[len] = str[len];
		len--;
	}
	return (ret);
}
