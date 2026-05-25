/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:09:05 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 17:09:07 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"

static void	apply_step(t_visu *visu, int step_index, int reverse);
static void	set_ant_room(t_visu *visu, int ant_id, t_room *room);
static void	add_active_ant(t_visu *visu, int ant_id);
static void	remove_active_ant(t_visu *visu, int ant_id);

void	timeline_reset(t_visu *visu) {
	int	i;

	i = 1;
	while (i <= visu->farm->ants)
	{
		visu->anim.ants[i].room = visu->farm->start;
		visu->anim.ants[i].arrived = 0;
		visu->anim.ant_status[i] = ANT_START;
		visu->anim.active_pos[i] = -1;
		i++;
	}
	visu->anim.current_slide = 0;
	visu->anim.start_count = visu->farm->ants;
	visu->anim.end_count = 0;
	visu->anim.active_count = 0;
}

void	timeline_next(t_visu *visu) {
	if (visu->anim.current_slide >= visu->anim.step_count)
		return ;
	apply_step(visu, visu->anim.current_slide, 0);
	visu->anim.current_slide++;
}

void	timeline_prev(t_visu *visu) {
	if (visu->anim.current_slide <= 0)
		return ;
	visu->anim.current_slide--;
	apply_step(visu, visu->anim.current_slide, 1);
}

static void	apply_step(t_visu *visu, int step_index, int reverse) {
	t_step	*step;
	t_move	*move;
	size_t	i;

	step = (t_step *)visu->anim.steps.data[step_index];
	i = 0;
	while (i < step->moves.count)
	{
		move = (t_move *)step->moves.data[i];
		if (reverse)
			set_ant_room(visu, move->ant_id, move->from);
		else
			set_ant_room(visu, move->ant_id, move->to);
		i++;
	}
}

static void	set_ant_room(t_visu *visu, int ant_id, t_room *room) {
	t_room	*old;

	old = visu->anim.ants[ant_id].room;
	if (old == visu->farm->start)
		visu->anim.start_count--;
	else if (old == visu->farm->end)
		visu->anim.end_count--;
	else
		remove_active_ant(visu, ant_id);
	visu->anim.ants[ant_id].room = room;
	if (room == visu->farm->start)
	{
		visu->anim.start_count++;
		visu->anim.ant_status[ant_id] = ANT_START;
		visu->anim.ants[ant_id].arrived = 0;
	}
	else if (room == visu->farm->end)
	{
		visu->anim.end_count++;
		visu->anim.ant_status[ant_id] = ANT_END;
		visu->anim.ants[ant_id].arrived = 1;
	}
	else
	{
		add_active_ant(visu, ant_id);
		visu->anim.ant_status[ant_id] = ANT_ACTIVE;
		visu->anim.ants[ant_id].arrived = 0;
	}
}

static void	add_active_ant(t_visu *visu, int ant_id) {
	if (visu->anim.active_pos[ant_id] != -1)
		return ;
	visu->anim.active_pos[ant_id] = visu->anim.active_count;
	visu->anim.active_ants[visu->anim.active_count] = ant_id;
	visu->anim.active_count++;
}

static void	remove_active_ant(t_visu *visu, int ant_id) {
	int	pos;
	int	last_ant;

	pos = visu->anim.active_pos[ant_id];
	if (pos == -1)
		return ;
	last_ant = visu->anim.active_ants[visu->anim.active_count - 1];
	visu->anim.active_ants[pos] = last_ant;
	visu->anim.active_pos[last_ant] = pos;
	visu->anim.active_count--;
	visu->anim.active_pos[ant_id] = -1;
}