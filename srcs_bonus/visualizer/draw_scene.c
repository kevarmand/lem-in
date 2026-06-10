/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:38:03 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/27 17:34:01 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"
#include "error.h"

static void	draw_terminal_room(SDL_Renderer *renderer, t_visu *visu,
				t_room *room, uint32_t color);
static uint32_t	get_room_color(t_visu *visu, t_room *room);

int	draw_scene(SDL_Renderer *renderer, t_visu *visu)
{
	Uint64	start;
	Uint64	part;
	Uint64	end;
	int		rebuilt;

	start = SDL_GetPerformanceCounter();
	rebuilt = 0;
	if (visu->background.dirty)
	{
		if (background_rebuild(renderer, visu))
			return (ERR_UNKNOWN);
		rebuilt = 1;
	}
	SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
	SDL_RenderClear(renderer);
	background_render(renderer, visu);
	part = SDL_GetPerformanceCounter();
	draw_dynamic_terminals(renderer, visu);
	end = SDL_GetPerformanceCounter();
	visu->profile.terminals_ms = profile_elapsed_ms(part, end);
	part = SDL_GetPerformanceCounter();
	draw_ants(renderer, visu);
	end = SDL_GetPerformanceCounter();
	visu->profile.ants_ms = profile_elapsed_ms(part, end);
	part = SDL_GetPerformanceCounter();
	panel_draw(renderer, visu);
	end = SDL_GetPerformanceCounter();
	visu->profile.hud_ms = profile_elapsed_ms(part, end);
	SDL_RenderPresent(renderer);
	end = SDL_GetPerformanceCounter();
	visu->profile.frame_ms = profile_elapsed_ms(start, end);
	if (rebuilt && visu->profile.enabled)
	{
		fprintf(stderr,
			"frame %.2f ms | bg %.2f ms | static %.2f ms | links %.2f ms | rooms %.2f ms | prep %.2f ms | prep_l %.2f ms | draw_l %.2f ms | prep_r %.2f ms | draw_r %.2f ms | texts %.2f ms | lines %d | circles %d | terms %.2f ms | ants %.2f ms | hud %.2f ms\n",
			visu->profile.frame_ms,
			visu->profile.background_ms,
			visu->profile.static_map_ms,
			visu->profile.links_ms,
			visu->profile.rooms_ms,
			visu->profile.prepare_ms,
			visu->profile.prepare_links_ms,
			visu->profile.draw_links_ms,
			visu->profile.prepare_rooms_ms,
			visu->profile.draw_rooms_ms,
			visu->profile.draw_texts_ms,
			visu->background.line_count,
			visu->background.circle_count,
			visu->profile.terminals_ms,
			visu->profile.ants_ms,
			visu->profile.hud_ms);
	}
	return (ERR_NO_ERROR);
}

void	draw_dynamic_terminals(SDL_Renderer *renderer, t_visu *visu)
{
	draw_terminal_room(renderer, visu, visu->farm->start,
		get_room_color(visu, visu->farm->start));
	draw_terminal_room(renderer, visu, visu->farm->end,
		get_room_color(visu, visu->farm->end));
}

static void	draw_terminal_room(SDL_Renderer *renderer, t_visu *visu,
	t_room *room, uint32_t color)
{
	int	x;
	int	y;
	int	w;
	int	h;

	logical_to_pixel(room->x, room->y, &visu->camera, &x, &y);
	w = visu_terminal_width(visu);
	h = visu_terminal_height(visu);
	boxColor(renderer, x - w / 2, y - h / 2, x + w / 2, y + h / 2,
		color);
	rectangleColor(renderer, x - w / 2, y - h / 2, x + w / 2,
		y + h / 2, COLOR_ROOM_BORDER);
	if (visu->settings.show_room_names)
		stringColor(renderer, x - w / 2 + 6, y - 14, room->name,
			COLOR_TEXT);
}

static uint32_t	get_room_color(t_visu *visu, t_room *room)
{
	if (room == visu->farm->start)
		return (visu_terminal_color(visu, visu->anim.start_count));
	if (room == visu->farm->end)
		return (visu_terminal_color(visu, visu->anim.end_count));
	return (COLOR_ROOM);
}