/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rooms.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:41:12 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/01 14:05:20 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"

static int	parse_room_line(t_farm *farm, char *line, int cmd);
static int	fill_room_parse(char *line, t_room_parse *info);
static int	validate_room_parse(t_farm *farm, t_room_parse *info);
static int	create_room(t_farm *farm, t_room_parse *info, int cmd);
static int	handle_room_cmd(int *err, char **line, t_farm *farm, int *cmd);

int	parse_rooms(int *err, char **line, t_farm *farm, int *cmd) {
	if (*err || !*line)
		return (0);
	if (parse_is_start_cmd(*line) || parse_is_end_cmd(*line))
		return (handle_room_cmd(err, line, farm, cmd));
	if ((*line)[0] == '#' && (*line)[1] == '#')
	{
		*err = ERR_ROOM;
		parse_consume_line(line);
		return (1);
	}
	if (parse_is_comment(*line))
	{
		if (*cmd != PARSE_CMD_NONE)
			*err = ERR_ROOM;
		parse_consume_line(line);
		return (1);
	}
	if (parse_count_char(*line, ' ') == 0)
	{
		if (*cmd != PARSE_CMD_NONE)
		{
			*err = ERR_ROOM;
			parse_consume_line(line);
			return (1);
		}
		return (0);
	}
	*err = parse_room_line(farm, *line, *cmd);
	*cmd = PARSE_CMD_NONE;
	*line = NULL;
	return (1);
}

static int	handle_room_cmd(int *err, char **line, t_farm *farm, int *cmd) {
	if (*cmd != PARSE_CMD_NONE)
		*err = ERR_ROOM;
	else if (parse_is_start_cmd(*line))
	{
		if (farm->start)
			*err = ERR_START_END;
		else
			*cmd = PARSE_CMD_START;
	}
	else
	{
		if (farm->end)
			*err = ERR_START_END;
		else
			*cmd = PARSE_CMD_END;
	}
	parse_consume_line(line);
	return (1);
}

static int	parse_room_line(t_farm *farm, char *line, int cmd) {
	t_room_parse	info;
	int				err;

	err = fill_room_parse(line, &info);
	if (!err)
		err = validate_room_parse(farm, &info);
	if (!err)
		err = create_room(farm, &info, cmd);
	if (err)
		free(line);
	return (err);
}

static int	fill_room_parse(char *line, t_room_parse *info) {
	char	*first;
	char	*second;

	if (parse_count_char(line, ' ') != 2)
		return (ERR_ROOM);
	first = ft_strchr(line, ' ');
	second = ft_strchr(first + 1, ' ');
	*first = '\0';
	*second = '\0';
	info->name = line;
	info->x_str = first + 1;
	info->y_str = second + 1;
	parse_strip_newline(info->y_str);
	return (ERR_NO_ERROR);
}

static int	validate_room_parse(t_farm *farm, t_room_parse *info) {
	char	*coord_key;
	char	*ptr;

	if (!info->name[0] || !info->x_str[0] || !info->y_str[0])
		return (ERR_ROOM);
	if (info->name[0] == 'L' || info->name[0] == '#')
		return (ERR_ROOM);
	if (ft_strchr(info->name, '-'))
		return (ERR_ROOM);
	ptr = info->name;
	while (*ptr)
	{
		if (!ft_isalnum(*ptr) && *ptr != '_')
			return (ERR_ROOM);
		ptr++;
	}
	if (custom_atoi(info->x_str, &info->x))
		return (ERR_ROOM);
	if (custom_atoi(info->y_str, &info->y))
		return (ERR_ROOM);
	coord_key = make_coord_key(info->x, info->y);
	if (!coord_key)
		return (ERR_MALLOC);
	if (hashmap_get(farm->rooms_by_name, info->name)
		|| hashmap_get(farm->rooms_by_coord, coord_key))
	{
		free(coord_key);
		return (ERR_ROOM);
	}
	free(coord_key);
	return (ERR_NO_ERROR);
}

static int	create_room(t_farm *farm, t_room_parse *info, int cmd) {
	t_room	*room;
	int		err;

	room = malloc(sizeof(*room));
	if (!room)
		return (ERR_MALLOC);
	room->id = (int)farm->rooms.count;
	room->name = info->name;
	room->x = info->x;
	room->y = info->y;
	if (vector_init(&room->neighbors, 4))
	{
		free(room);
		return (ERR_MALLOC);
	}
	err = farm_add_room(farm, room);
	if (err)
	{
		room_destroy(room);
		return (err);
	}
	if (cmd == PARSE_CMD_START)
		farm->start = room;
	else if (cmd == PARSE_CMD_END)
		farm->end = room;
	return (ERR_NO_ERROR);
}