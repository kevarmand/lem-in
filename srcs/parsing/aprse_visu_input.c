/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aprse_visu_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:42:58 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/27 14:48:06 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "visualizer.h"
#include "libft.h"

#include <stdlib.h>
#include "visualizer.h"
#include "libft.h"

static int		visu_parse_init(t_farm *farm, t_visu *visu);
static int		anim_init(t_farm *farm, t_anim *anim);
static void		read_next_line(char **line);
static uint32_t	ant_color(int id);

int	parse_visu_input(t_farm *farm, t_visu *visu)
{
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
	if (line)
		free(line);
	farm->error = err;
	return (err);
}

static double	profile_elapsed_ms(Uint64 start, Uint64 end) {
	return ((double)(end - start) * 1000.0
		/ (double)SDL_GetPerformanceFrequency());
}

static int	visu_parse_init(t_farm *farm, t_visu *visu) {
	ft_bzero(visu, sizeof(*visu));
	visu->farm = farm;
	visu->settings.show_room_names = 0;
	visu->settings.show_ant_ids = 0;
	visu->settings.show_links = 1;
	visu->settings.show_hud = 1;
	visu->settings.show_controls = 1;
	visu->profile.enabled = 0;
	visu->profile.last_print_ms = 0;
	visu->hover_ant = -1;
	visu->camera.win_width = WINDOW_WIDTH;
	visu->camera.win_height = WINDOW_HEIGHT;
	visu->layout.mode = LAYOUT_RAW;
	visu->background.dirty = 1;
	visu->background.width = WINDOW_WIDTH;
	visu->background.height = WINDOW_HEIGHT;
	if (vector_init(&visu->paths, 8))
		return (ERR_MALLOC);
	if (vector_init(&visu->layout.nodes, 64))
		return (ERR_MALLOC);
	if (vector_init(&visu->anim.steps, 128))
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

static int	anim_init(t_farm *farm, t_anim *anim)
{
	int	i;

	anim->ants = ft_calloc(farm->ants + 1, sizeof(*anim->ants));
	if (!anim->ants)
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
	anim->current_step = 0;
	anim->time = 0.0;
	anim->step_duration = 0.45;
	anim->paused = 0;
	return (ERR_NO_ERROR);
}

static uint32_t	ant_color(int id)
{
	unsigned int	x;

	x = (unsigned int)id;
	x = x * 1103515245u + 12345u;
	return (((x & 0x00FFFFFFu) << 8) | 0xFFu);
}

static void	read_next_line(char **line)
{
	*line = get_next_line(0);
}