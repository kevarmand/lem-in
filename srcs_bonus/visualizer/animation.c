/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 13:16:12 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 13:27:51 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "visualizer.h"

static void	transition_start_next(t_visu *visu);
static void	transition_finish(t_visu *visu);
static void	transition_clear(t_visu *visu);
static void	transition_add_static_ants(t_visu *visu);
static void	transition_add_moves(t_visu *visu, int step_index);
static void	transition_add_ant(t_visu *visu, int ant_id, t_room *from,
				t_room *to);

void	anim_toggle_play(t_visu *visu)
{
	if (visu->anim.playing)
	{
		anim_update(visu);
		visu->anim.playing = 0;
		return ;
	}
	if (visu->anim.current_slide >= visu->anim.step_count)
		timeline_reset(visu);
	if (!visu->anim.transition.active)
		transition_start_next(visu);
	if (!visu->anim.transition.active)
		return ;
	visu->anim.transition.start_ms = SDL_GetTicks()
		- (Uint32)(visu->anim.transition.progress
			* visu->anim.transition.duration_ms);
	visu->anim.playing = 1;
}

void	anim_update(t_visu *visu)
{
	Uint32	now;
	double	elapsed;

	if (!visu->anim.playing)
		return ;
	if (!visu->anim.transition.active)
		transition_start_next(visu);
	if (!visu->anim.transition.active)
	{
		visu->anim.playing = 0;
		return ;
	}
	now = SDL_GetTicks();
	elapsed = (double)(now - visu->anim.transition.start_ms);
	visu->anim.transition.progress = elapsed
		/ (double)visu->anim.transition.duration_ms;
	if (visu->anim.transition.progress < 1.0)
		return ;
	visu->anim.transition.progress = 1.0;
	transition_finish(visu);
	if (visu->anim.current_slide >= visu->anim.step_count)
		visu->anim.playing = 0;
	else
		transition_start_next(visu);
}

static void	transition_start_next(t_visu *visu)
{
	if (visu->anim.current_slide >= visu->anim.step_count)
		return ;
	transition_clear(visu);
	visu->anim.transition.active = 1;
	visu->anim.transition.direction = 1;
	visu->anim.transition.step_index = visu->anim.current_slide;
	visu->anim.transition.start_ms = SDL_GetTicks();
	visu->anim.transition.duration_ms = ANIM_STEP_DURATION_MS;
	visu->anim.transition.progress = 0.0;
	transition_add_static_ants(visu);
	transition_add_moves(visu, visu->anim.transition.step_index);
}

static void	transition_finish(t_visu *visu)
{
	if (visu->anim.transition.direction == 1)
		timeline_next(visu);
	else
		timeline_prev(visu);
	transition_clear(visu);
}

static void	transition_clear(t_visu *visu)
{
	int	i;
	int	ant_id;

	i = 0;
	while (i < visu->anim.transition.count)
	{
		ant_id = visu->anim.transition.ants[i].ant_id;
		visu->anim.transition.pos[ant_id] = -1;
		i++;
	}
	visu->anim.transition.active = 0;
	visu->anim.transition.direction = 0;
	visu->anim.transition.step_index = -1;
	visu->anim.transition.progress = 0.0;
	visu->anim.transition.count = 0;
}

static void	transition_add_static_ants(t_visu *visu)
{
	int			i;
	int			ant_id;
	t_room		*room;

	i = 0;
	while (i < visu->anim.active_count)
	{
		ant_id = visu->anim.active_ants[i];
		room = visu->anim.ants[ant_id].room;
		transition_add_ant(visu, ant_id, room, room);
		i++;
	}
}

static void	transition_add_moves(t_visu *visu, int step_index)
{
	t_step	*step;
	t_move	*move;
	size_t	i;

	step = (t_step *)visu->anim.steps.data[step_index];
	i = 0;
	while (i < step->moves.count)
	{
		move = (t_move *)step->moves.data[i];
		transition_add_ant(visu, move->ant_id, move->from, move->to);
		i++;
	}
}

static void	transition_add_ant(t_visu *visu, int ant_id, t_room *from,
	t_room *to)
{
	t_transition	*transition;
	int				pos;

	transition = &visu->anim.transition;
	pos = transition->pos[ant_id];
	if (pos != -1)
	{
		transition->ants[pos].to = to;
		return ;
	}
	pos = transition->count;
	transition->pos[ant_id] = pos;
	transition->ants[pos].ant_id = ant_id;
	transition->ants[pos].from = from;
	transition->ants[pos].to = to;
	transition->ants[pos].color = visu->anim.ants[ant_id].color;
	transition->count++;
}