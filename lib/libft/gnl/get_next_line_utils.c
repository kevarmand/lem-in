/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:20:36 by kearmand          #+#    #+#             */
/*   Updated: 2024/11/29 11:12:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"

int	ft_strnchr(char *s, char c, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (c == *s)
			return (1 + i);
		i++;
		s++;
	}
	return (-1);
}

static int	ft_fill_full(char **line, t_gnl *buf, int index)
{
	char	*tmp;
	int		j;
	int		i;

	tmp = malloc(1 + index + ft_strlen(*line));
	if (tmp == 0)
	{
		free(*line);
		*line = 0;
		return (1);
	}
	i = -1;
	j = -1;
	while ((*line)[++j])
		tmp[j] = (*line)[j];
	while (++i < index)
		tmp[i + j] = buf->buffer[buf->i + i];
	tmp[i + j] = 0;
	free(*line);
	*line = tmp;
	return (0);
}

int	ft_fill_line(char **line, t_gnl *buf, int index)
{
	int		i;

	i = 0;
	if (index == -1)
		index = buf->size - buf->i;
	if (*line == 0)
		*line = malloc(index + 1);
	else
		return (ft_fill_full(line, buf, index));
	if (*line == 0)
		return (1);
	while (i < index)
	{
		(*line)[i] = buf->buffer[buf->i + i];
		i++;
	}
	(*line)[i] = 0;
	return (0);
}
