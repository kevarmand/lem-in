/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_visu_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:41:15 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 18:59:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "visualizer.h"
#include "libft.h"

static int		visu_parse_init(t_farm *farm, t_visu *visu);
static int		anim_init(t_farm *farm, t_anim *anim);
static void		read_next_line(char **line);
static uint32_t	ant_color(int id);

int	parse_visu_input(t_farm *farm, t_visu *visu) {
	char	*line;
	int		err;
	int		cmd;

	err = visu_parse_init(farm, visu);
	if (err)
		return (err);
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
	if (!err)
		err = parse_check_farm(farm);
	if (!err)
		err = anim_init(farm, &visu->anim);
	while (!err && parse_move_line(&err, &line, farm, visu))
		read_next_line(&line);
	if (!err && line)
		err = ERR_MOVE;
	if (!err)
		timeline_reset(visu);
	if (line)
		free(line);
	farm->error = err;
	return (err);
}

static int	visu_parse_init(t_farm *farm, t_visu *visu)
{
	ft_bzero(visu, sizeof(*visu));
	visu->farm = farm;
	visu->settings.show_room_names = 1;
	visu->settings.show_ant_ids = 0;
	visu->settings.show_links = 1;
	visu->settings.show_hud = 1;
	visu->settings.show_controls = 1;
	visu->hover_ant = -1;
	visu->camera.win_width = WINDOW_WIDTH;
	visu->camera.win_height = WINDOW_HEIGHT;
	visu->layout.mode = LAYOUT_RAW;
	if (vector_init(&visu->paths, 8))
		return (ERR_MALLOC);
	if (vector_init(&visu->layout.nodes, 64))
		return (ERR_MALLOC);
	if (vector_init(&visu->anim.steps, 128))
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

static int	anim_init(t_farm *farm, t_anim *anim) {
	int	i;

	anim->ants = ft_calloc(farm->ants + 1, sizeof(*anim->ants));
	if (!anim->ants)
		return (ERR_MALLOC);
	anim->active_ants = ft_calloc(farm->ants + 1, sizeof(*anim->active_ants));
	if (!anim->active_ants)
		return (ERR_MALLOC);
	anim->active_pos = ft_calloc(farm->ants + 1, sizeof(*anim->active_pos));
	if (!anim->active_pos)
		return (ERR_MALLOC);
	anim->ant_status = ft_calloc(farm->ants + 1, sizeof(*anim->ant_status));
	if (!anim->ant_status)
		return (ERR_MALLOC);
	i = 1;
	while (i <= farm->ants)
	{
		anim->ants[i].id = i;
		anim->ants[i].room = farm->start;
		anim->ants[i].path_id = -1;
		anim->ants[i].color = ant_color(i);
		anim->ants[i].arrived = 0;
		i++;
	}
	anim->step_count = 0;
	anim->current_slide = 0;
	anim->start_count = farm->ants;
	anim->end_count = 0;
	anim->active_count = 0;
	anim->time = 0.0;
	anim->step_duration = 0.45;
	anim->paused = 1;
	return (ERR_NO_ERROR);
}

static uint32_t	ant_color(int id)
{
	unsigned int	x;
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	x = (unsigned int)id;
	x = x * 1103515245u + 12345u;
	r = (x >> 16) & 0xFFu;
	g = (x >> 8) & 0xFFu;
	b = x & 0xFFu;
	return (GFX_COLOR(r, g, b, 255));
}

static void	read_next_line(char **line) {
	*line = get_next_line(0);
}