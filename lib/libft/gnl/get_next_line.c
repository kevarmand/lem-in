/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:14:04 by kearmand          #+#    #+#             */
/*   Updated: 2025/02/14 17:04:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>

static int	ft_read(int fd, t_gnl *buf)
{
	int	i;

	i = 0;
	while (i < BUFFER_SIZE)
	{
		buf->buffer[i] = 0;
		i++;
	}
	buf->size = read(fd, buf->buffer, BUFFER_SIZE);
	buf->i = 0;
	return (buf->size);
}

static char	*ft_free(t_gnl	*buf, char *line, int type)
{
	if (buf == 0)
		return (0);
	if (type != 0)
	{
		if (line != 0)
			free(line);
		return (0);
	}
	return (line);
}

static int	ft_init(int fd)
{
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (1);
	return (0);
}

char	*get_next_line(int fd)
{
	static t_gnl	buf;
	int				index;
	char			*line;

	line = 0;
	if (ft_init(fd))
		return (0);
	if (buf.i >= buf.size)
		if (ft_read(fd, &buf) <= 0)
			return (ft_free(&(buf), line, 0));
	index = ft_strnchr(buf.buffer + buf.i,
			'\n', buf.size - buf.i);
	while (index == -1)
	{
		if (ft_fill_line(&line, &buf, index))
			return (ft_free(&(buf), line, 0));
		if (ft_read(fd, &buf) <= 0)
			return (ft_free(&(buf), line, -buf.size));
		index = ft_strnchr(buf.buffer + buf.i, '\n', buf.size);
	}
	if (ft_fill_line(&line, &buf, index))
		return (ft_free(&(buf), line, 0));
	buf.i += index;
	return (line);
}
