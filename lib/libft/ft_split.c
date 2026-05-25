/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:58:02 by kearmand          #+#    #+#             */
/*   Updated: 2025/02/18 14:06:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	nb_word(const char *s, char c, char ***ret)
{
	int	nb;

	if (!s)
		return (-1);
	nb = 0;
	while (*s && *s == c)
		s++;
	while (*s)
	{
		while (*s && *s != c)
			s++;
		while (*s && *s == c)
			s++;
		nb++;
	}
	*ret = malloc(sizeof(char *) * (1 + nb));
	if (*ret == 0)
		return (-1);
	return (nb);
}

static int	ft_strlenword(char const *s, char c)
{
	int	i;

	i = 0;
	while (*s && *s != c)
	{
		i++;
		s++;
	}
	return (i);
}

static char	**dest_list(char **ret, int k)
{
	int	i;

	i = 0;
	while (i < k)
	{
		free(ret[i]);
		i++;
	}
	free(ret);
	return (0);
}

char	**ft_split(const char *s, char c)
{
	char	**ret;
	int		nb;
	int		k;
	int		i;

	nb = nb_word(s, c, &ret);
	if (nb == -1)
		return (0);
	k = -1;
	while (++k < nb)
	{
		while (*s && *s == c)
			s++;
		ret[k] = malloc(1 + ft_strlenword(s, c));
		if (ret[k] == 0)
			return (dest_list(ret, k));
		i = 0;
		while (*s && c != *s)
			ret[k][i++] = *(s++);
		ret[k][i] = 0;
	}
	ret[nb] = 0;
	return (ret);
}
