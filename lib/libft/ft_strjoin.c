/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:28:18 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/13 09:32:45 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*str;
	size_t	ls1;
	size_t	ls2;

	if (s1 == 0 || s2 == 0)
		return (0);
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	str = ft_calloc(1, 1 + ls1 + ls2);
	if (str == 0)
		return (0);
	ft_memcpy(str, s1, ls1);
	ft_memcpy(str + ls1, s2, ls2);
	return (str);
}
