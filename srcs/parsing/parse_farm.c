/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_farm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:42:43 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:57:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"
#include "libft.h"

static void	read_next_line(char **line);

int	parse_farm(t_farm *farm)
{
	char	*line;
	int		err;
	int		cmd;

	err = ERR_NO_ERROR;
	cmd = PARSE_CMD_NONE;
	line = get_next_line(0);
	while (!err && parse_ants(&err, &line, farm))
		read_next_line(&line);
	while (!err && parse_rooms(&err, &line, farm, &cmd))
		read_next_line(&line);
	if (!err && cmd != PARSE_CMD_NONE)
		err = ERR_ROOM;
	while (!err && parse_links(&err, &line, farm))
		read_next_line(&line);
	if (!err && line)
		err = ERR_FORMAT;
	if (!err)
		err = parse_check_farm(farm);
	if (line)
		free(line);
	farm->error = err;
	return (err);
}

static void	read_next_line(char **line)
{
	*line = get_next_line(0);
}