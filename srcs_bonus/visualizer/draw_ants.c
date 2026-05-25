/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:12:07 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 19:00:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "visualizer.h"

static void	draw_start_end_counters(SDL_Renderer *renderer, t_visu *visu);
static void	draw_active_ants(SDL_Renderer *renderer, t_visu *visu);
static void	draw_terminal_counter(SDL_Renderer *renderer, t_visu *visu,
				t_room *room, int count);
static void	draw_ant_id(SDL_Renderer *renderer, t_visu *visu, t_ant_state *ant,
				int x, int y);

void	draw_ants(SDL_Renderer *renderer, t_visu *visu)
{
	draw_start_end_counters(renderer, visu);
	draw_active_ants(renderer, visu);
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
	int			x;
	int			y;
	int			radius;

	i = 0;
	radius = logical_radius_to_pixel(0.07, &visu->camera);
	while (i < visu->anim.active_count)
	{
		id = visu->anim.active_ants[i];
		ant = &visu->anim.ants[id];
		room = ant->room;
		logical_to_pixel(room->x, room->y, &visu->camera, &x, &y);
		filledCircleColor(renderer, x, y, radius, ant->color);
		aacircleColor(renderer, x, y, radius, COLOR_ROOM_BORDER);
		if (visu->settings.show_ant_ids)
			draw_ant_id(renderer, visu, ant, x, y);
		i++;
	}
}

static void	draw_terminal_counter(SDL_Renderer *renderer, t_visu *visu,
	t_room *room, int count)
{
	char	buffer[32];
	int		x;
	int		y;

	logical_to_pixel(room->x, room->y, &visu->camera, &x, &y);
	snprintf(buffer, sizeof(buffer), "%d", count);
	stringColor(renderer, x - 8, y + 4, buffer, COLOR_TEXT);
}

static void	draw_ant_id(SDL_Renderer *renderer, t_visu *visu, t_ant_state *ant,
	int x, int y)
{
	char	buffer[16];

	(void)visu;
	snprintf(buffer, sizeof(buffer), "%d", ant->id);
	stringColor(renderer, x + 8, y - 4, buffer, COLOR_TEXT);
}