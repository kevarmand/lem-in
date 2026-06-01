#include <stdlib.h>
#include "visualizer.h"
#include "libft.h"

static int	visu_parse_init(t_farm *farm, t_visu *visu);
static int	anim_init(t_farm *farm, t_anim *anim);
static int	path_colors_init(t_farm *farm, t_visu *visu);
static void	read_next_line(char **line);

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
		err = path_colors_init(farm, visu);
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
	visu->settings.show_room_names = 0;
	visu->settings.show_ant_ids = 0;
	visu->settings.show_links = 1;
	visu->settings.show_unused_elements = 1;
	visu->settings.color_paths = 1;
	visu->settings.show_overlay = 1;
	visu->profile.enabled = 1;
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
	anim->active_ants = ft_calloc(farm->ants + 1,
			sizeof(*anim->active_ants));
	if (!anim->active_ants)
		return (ERR_MALLOC);
	anim->active_pos = ft_calloc(farm->ants + 1, sizeof(*anim->active_pos));
	if (!anim->active_pos)
		return (ERR_MALLOC);
	anim->ant_status = ft_calloc(farm->ants + 1, sizeof(*anim->ant_status));
	if (!anim->ant_status)
		return (ERR_MALLOC);
	anim->transition.ants = ft_calloc(farm->ants + 1,
			sizeof(*anim->transition.ants));
	if (!anim->transition.ants)
		return (ERR_MALLOC);
	anim->transition.pos = ft_calloc(farm->ants + 1,
			sizeof(*anim->transition.pos));
	if (!anim->transition.pos)
		return (ERR_MALLOC);
	i = 1;
	while (i <= farm->ants)
	{
		anim->ants[i].id = i;
		anim->ants[i].room = farm->start;
		anim->ants[i].path_id = -1;
		anim->ants[i].color = COLOR_TEXT;
		anim->ants[i].arrived = 0;
		anim->active_pos[i] = -1;
		anim->transition.pos[i] = -1;
		i++;
	}
	anim->step_count = 0;
	anim->current_slide = 0;
	anim->start_count = farm->ants;
	anim->end_count = 0;
	anim->active_count = 0;
	anim->playing = 0;
	anim->transition.active = 0;
	anim->transition.direction = 0;
	anim->transition.step_index = -1;
	anim->transition.start_ms = 0;
	anim->transition.duration_ms = ANIM_STEP_DURATION_MS;
	anim->transition.progress = 0.0;
	anim->transition.count = 0;
	anim->speed_index = ANIM_SPEED_DEFAULT_INDEX;
	anim->step_duration_ms = ANIM_STEP_DURATION_MS;
	return (ERR_NO_ERROR);
}

static int	path_colors_init(t_farm *farm, t_visu *visu)
{
	size_t	i;

	visu->path_color_index = ft_calloc(farm->rooms.count, sizeof(int));
	if (!visu->path_color_index)
		return (ERR_MALLOC);
	i = 0;
	while (i < farm->rooms.count)
	{
		visu->path_color_index[i] = -1;
		i++;
	}
	visu->path_count = 0;
	return (ERR_NO_ERROR);
}

static void	read_next_line(char **line)
{
	*line = get_next_line(0);
}