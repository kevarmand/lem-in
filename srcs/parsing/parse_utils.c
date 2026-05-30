/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:40:04 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/30 15:58:54 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"

static int 	is_path_from_start_to_end(t_farm *farm)
{
	int			*visited;
	t_vector	queue;
	t_room		*current;
	t_room		*neightbor;
	int			max_id;
	int			found;

	max_id = 0;
	for (size_t i = 0; i < farm->rooms.count; i++)
	{
		if (((t_room *)farm->rooms.data[i])->id > max_id)
			max_id = ((t_room *)farm->rooms.data[i])->id;
	}
	visited = ft_calloc(max_id + 1, sizeof(int));
	if (!visited)
		return (0);
	vector_init(&queue, 10);
	vector_push_back(&queue, farm->start);
	visited[farm->start->id] = 1;
	found = 0;
	size_t i;
	while (queue.count > 0)
	{
		current = (t_room *)queue.data[0];
		vector_remove(&queue, 0);
		if (current == farm->end)
		{
			found = 1;
			break ;
		}
		i = 0;
		while (i < farm->links.count)
		{
			neightbor = NULL;
			if (((t_link *)farm->links.data[i])->a == current)
				neightbor = ((t_link *)farm->links.data[i])->b;
			else if (((t_link *)farm->links.data[i])->b == current)
				neightbor = ((t_link *)farm->links.data[i])->a;
			if (neightbor && !visited[neightbor->id])
			{
				visited[neightbor->id] = 1;
				vector_push_back(&queue, neightbor);
			}
			i++;
		}
	}
	vector_destroy(&queue);
	free(visited);
	return(found);
}

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