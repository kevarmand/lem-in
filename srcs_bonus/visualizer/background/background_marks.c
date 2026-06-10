#include <stdlib.h>
#include "visualizer.h"
#include "error.h"

static void	background_mark_move(t_visu *visu, t_move *move);
static int	background_link_slot(t_visu *visu, t_link *link);

int	background_init_marks(t_visu *visu)
{
	size_t	i;

	visu->background.room_used = malloc(sizeof(int) * visu->farm->rooms.count);
	if (!visu->background.room_used)
		return (ERR_MALLOC);
	visu->background.room_color = malloc(sizeof(uint32_t)
			* visu->farm->rooms.count);
	if (!visu->background.room_color)
		return (ERR_MALLOC);
	visu->background.link_used = malloc(sizeof(int) * visu->farm->links.count);
	if (!visu->background.link_used)
		return (ERR_MALLOC);
	visu->background.link_color = malloc(sizeof(uint32_t)
			* visu->farm->links.count);
	if (!visu->background.link_color)
		return (ERR_MALLOC);
	i = 0;
	while (i < visu->farm->rooms.count)
	{
		visu->background.room_used[i] = 0;
		visu->background.room_color[i] = COLOR_ROOM_BORDER;
		i++;
	}
	i = 0;
	while (i < visu->farm->links.count)
	{
		visu->background.link_used[i] = 0;
		visu->background.link_color[i] = COLOR_LINK;
		i++;
	}
	return (ERR_NO_ERROR);
}

int	background_init_link_ids(t_visu *visu)
{
	t_link	*link;
	int		i;
	int		slot;

	visu->background.link_id_capacity = (int)visu->farm->links.count * 2 + 1;
	visu->background.link_ids = malloc(sizeof(*visu->background.link_ids)
			* visu->background.link_id_capacity);
	if (!visu->background.link_ids)
		return (ERR_MALLOC);
	i = 0;
	while (i < visu->background.link_id_capacity)
	{
		visu->background.link_ids[i].link = NULL;
		visu->background.link_ids[i].id = -1;
		i++;
	}
	i = 0;
	while (i < (int)visu->farm->links.count)
	{
		link = (t_link *)visu->farm->links.data[i];
		slot = background_link_slot(visu, link);
		visu->background.link_ids[slot].link = link;
		visu->background.link_ids[slot].id = i;
		i++;
	}
	return (ERR_NO_ERROR);
}

int	background_mark_solution(t_visu *visu)
{
	t_step	*step;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < visu->anim.steps.count)
	{
		step = (t_step *)visu->anim.steps.data[i];
		j = 0;
		while (j < step->moves.count)
		{
			background_mark_move(visu, (t_move *)step->moves.data[j]);
			j++;
		}
		i++;
	}
	return (ERR_NO_ERROR);
}

int	background_link_id(t_visu *visu, t_link *link)
{
	int	slot;

	slot = background_link_slot(visu, link);
	if (!visu->background.link_ids[slot].link)
		return (-1);
	return (visu->background.link_ids[slot].id);
}

static void	background_mark_move(t_visu *visu, t_move *move)
{
	uint32_t	color;
	int			link_id;

	color = visu_ant_path_color(visu, move->path_id, 0);
	visu->background.room_used[move->from->id] = 1;
	visu->background.room_color[move->from->id] = color;
	visu->background.room_used[move->to->id] = 1;
	visu->background.room_color[move->to->id] = color;
	link_id = background_link_id(visu, move->link);
	if (link_id < 0)
		return ;
	visu->background.link_used[link_id] = 1;
	visu->background.link_color[link_id] = color;
}

static int	background_link_slot(t_visu *visu, t_link *link)
{
	uintptr_t	hash;
	int			slot;

	hash = (uintptr_t)link;
	slot = (int)((hash >> 4) % (uintptr_t)visu->background.link_id_capacity);
	while (visu->background.link_ids[slot].link
		&& visu->background.link_ids[slot].link != link)
	{
		slot++;
		if (slot == visu->background.link_id_capacity)
			slot = 0;
	}
	return (slot);
}