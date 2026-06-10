/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_links.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:42:30 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:42:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"

static int	parse_link_line(t_farm *farm, char *line);
static int	fill_link_parse(t_farm *farm, char *line, t_link_parse *info);
static int	create_link(t_farm *farm, t_link_parse *info);

int	parse_links(int *err, char **line, t_farm *farm) {
	int	dashes;

	if (*err || !*line)
		return (0);
	if (parse_is_comment(*line))
	{
		parse_consume_line(line);
		return (1);
	}
	dashes = parse_count_char(*line, '-');
	if (dashes == 0)
		return (0);
	if (dashes != 1 || parse_count_char(*line, ' ') != 0)
	{
		*err = ERR_LINK;
		parse_consume_line(line);
		return (1);
	}
	*err = parse_link_line(farm, *line);
	*line = NULL;
	return (1);
}

static int	parse_link_line(t_farm *farm, char *line) {
	t_link_parse	info;
	int				err;

	err = fill_link_parse(farm, line, &info);
	if (!err && hashmap_get(farm->links_by_key, info.key))
	{
		free(info.key);
		free(line);
		return (ERR_LINK);
	}
	if (!err)
		err = create_link(farm, &info);
	free(line);
	return (err);
}

static int	fill_link_parse(t_farm *farm, char *line, t_link_parse *info) {
	char	*dash;

	parse_strip_newline(line);
	dash = ft_strchr(line, '-');
	*dash = '\0';
	info->name_a = line;
	info->name_b = dash + 1;
	if (!info->name_a[0] || !info->name_b[0])
		return (ERR_LINK);
	info->room_a = hashmap_get(farm->rooms_by_name, info->name_a);
	info->room_b = hashmap_get(farm->rooms_by_name, info->name_b);
	if (!info->room_a || !info->room_b || info->room_a == info->room_b)
		return (ERR_LINK);
	info->key = make_link_key(info->name_a, info->name_b);
	if (!info->key)
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

static int	create_link(t_farm *farm, t_link_parse *info) {
	t_link	*link;
	int		err;

	link = malloc(sizeof(*link));
	if (!link)
	{
		free(info->key);
		return (ERR_MALLOC);
	}
	link->a = info->room_a;
	link->b = info->room_b;
	link->raw = info->key;
	err = farm_add_link(farm, link);
	if (err)
	{
		link_destroy(link);
		return (err);
	}
	return (ERR_NO_ERROR);
}