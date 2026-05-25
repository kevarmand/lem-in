/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:40:04 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:40:06 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"

int	parse_is_start_cmd(char *line) {
	if (!line)
		return (0);
	if (ft_strncmp(line, "##start", 7) != 0)
		return (0);
	return (line[7] == '\0' || line[7] == '\n');
}

int	parse_is_end_cmd(char *line) {
	if (!line)
		return (0);
	if (ft_strncmp(line, "##end", 5) != 0)
		return (0);
	return (line[5] == '\0' || line[5] == '\n');
}

int	parse_is_comment(char *line) {
	return (line && line[0] == '#');
}

int	parse_count_char(char *line, char c) {
	int	count;

	count = 0;
	while (*line && *line != '\n')
	{
		if (*line == c)
			count++;
		line++;
	}
	return (count);
}

void	parse_strip_newline(char *line) {
	size_t	len;

	if (!line)
		return ;
	len = ft_strlen(line);
	if (len && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

void	parse_consume_line(char **line) {
	free(*line);
	*line = NULL;
}

int	parse_check_farm(t_farm *farm) {
	if (farm->ants <= 0)
		return (ERR_ANTS);
	if (!farm->start || !farm->end)
		return (ERR_START_END);
	if (farm->rooms.count < 2)
		return (ERR_ROOM);
	if (farm->links.count == 0)
		return (ERR_LINK);
	return (ERR_NO_ERROR);
}