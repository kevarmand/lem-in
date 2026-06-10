/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:12:07 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 14:11:19 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include "visualizer.h"

static void	draw_start_end_counters(SDL_Renderer *renderer, t_visu *visu);
static void	draw_active_ants(SDL_Renderer *renderer, t_visu *visu);
static void	draw_transition_ants(SDL_Renderer *renderer, t_visu *visu);
static void	draw_terminal_counter(SDL_Renderer *renderer, t_visu *visu,
				t_room *room, int count);
static void	draw_ant_at(SDL_Renderer *renderer, t_visu *visu, uint32_t color,
				double x_log, double y_log, int ant_id);
static void	draw_ant_id(SDL_Renderer *renderer, int ant_id, int x, int y);

void	draw_ants(SDL_Renderer *renderer, t_visu *visu)
{
	if (visu->anim.transition.active)
		draw_transition_ants(renderer, visu);
	else
		draw_active_ants(renderer, visu);
	draw_start_end_counters(renderer, visu);
}

static void	draw_start_end_counters(SDL_Renderer *renderer, t_visu *visu)
{
	draw_terminal_counter(renderer, visu, visu->farm->start,
		visu->anim.start_count);
	draw_terminal_counter(renderer, visu, visu->farm->end,
		visu->anim.end_count);
}

static void	draw_active_ants(SDL_Renderer *renderer, t_visu *visu)
{
	t_ant_state	*ant;
	t_room		*room;
	int			id;
	int			i;

	i = 0;
	while (i < visu->anim.active_count)
	{
		id = visu->anim.active_ants[i];
		ant = &visu->anim.ants[id];
		room = ant->room;
		draw_ant_at(renderer, visu, ant->color, room->x, room->y, id);
		i++;
	}
}

static void	draw_transition_ants(SDL_Renderer *renderer, t_visu *visu)
{
	t_transition_ant	*ant;
	double				t;
	double				x_log;
	double				y_log;
	int					i;

	t = visu->anim.transition.progress;
	i = 0;
	while (i < visu->anim.transition.count)
	{
		ant = &visu->anim.transition.ants[i];
		x_log = ant->from->x + (ant->to->x - ant->from->x) * t;
		y_log = ant->from->y + (ant->to->y - ant->from->y) * t;
		draw_ant_at(renderer, visu, ant->color, x_log, y_log, ant->ant_id);
		i++;
	}
}

static void	draw_terminal_counter(SDL_Renderer *renderer, t_visu *visu,
	t_room *room, int count)
{
	char	buffer[32];
	int		x;
	int		y;
	int		w;

	logical_to_pixel(room->x, room->y, &visu->camera, &x, &y);
	w = visu_terminal_width(visu);
	snprintf(buffer, sizeof(buffer), "%d", count);
	stringColor(renderer, x - w / 2 + 4, y - 4, buffer, COLOR_TEXT);
}

static void	draw_ant_at(SDL_Renderer *renderer, t_visu *visu, uint32_t color,
	double x_log, double y_log, int ant_id)
{
	int	x;
	int	y;
	int	radius;

	logical_to_pixel(x_log, y_log, &visu->camera, &x, &y);
	radius = visu_ant_radius(visu);
	filledCircleColor(renderer, x, y, radius, color);
	aacircleColor(renderer, x, y, radius, COLOR_ROOM_BORDER);
	if (visu->settings.show_ant_ids)
		draw_ant_id(renderer, ant_id, x + radius + 3, y + 3);
}

static void	draw_ant_id(SDL_Renderer *renderer, int ant_id, int x, int y)
{
	char	buffer[16];

	snprintf(buffer, sizeof(buffer), "%d", ant_id);
	stringColor(renderer, x, y, buffer, COLOR_TEXT);
}