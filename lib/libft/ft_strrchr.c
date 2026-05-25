/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:46:41 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/11 16:41:35 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	const char	*save;

	save = 0;
	while (*s)
	{
		if (*s == (char)c)
			save = s;
		s++;
	}
	if ((char)c == 0)
		return ((char *)s);
	return ((char *)save);
}
