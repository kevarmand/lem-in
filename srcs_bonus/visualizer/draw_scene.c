/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:38:03 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 18:59:38 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "visualizer.h"

static void	draw_links(SDL_Renderer *renderer, t_visu *visu);
static void	draw_rooms(SDL_Renderer *renderer, t_visu *visu);
static void	draw_hud(SDL_Renderer *renderer, t_visu *visu);
static void	draw_controls(SDL_Renderer *renderer, t_visu *visu);
static void	draw_one_link(SDL_Renderer *renderer, t_visu *visu, t_link *link);
static void	draw_one_room(SDL_Renderer *renderer, t_visu *visu, t_room *room);
static void	draw_terminal_room(SDL_Renderer *renderer, t_visu *visu,
				t_room *room, uint32_t color);
static uint32_t	get_room_color(t_visu *visu, t_room *room);

void	draw_scene(SDL_Renderer *renderer, t_visu *visu)
{
	SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
	SDL_RenderClear(renderer);
	if (visu->settings.show_links)
		draw_links(renderer, visu);
	draw_rooms(renderer, visu);
	draw_ants(renderer, visu);
	if (visu->settings.show_hud)
		draw_hud(renderer, visu);
	if (visu->settings.show_controls)
		draw_controls(renderer, visu);
	SDL_RenderPresent(renderer);
}

static void	draw_links(SDL_Renderer *renderer, t_visu *visu)
{
	t_link	*link;
	size_t	i;

	i = 0;
	while (i < visu->farm->links.count)
	{
		link = (t_link *)visu->farm->links.data[i];
		draw_one_link(renderer, visu, link);
		i++;
	}
}

static void	draw_rooms(SDL_Renderer *renderer, t_visu *visu)
{
	t_room	*room;
	size_t	i;

	i = 0;
	while (i < visu->farm->rooms.count)
	{
		room = (t_room *)visu->farm->rooms.data[i];
		draw_one_room(renderer, visu, room);
		i++;
	}
}

static void	draw_one_link(SDL_Renderer *renderer, t_visu *visu, t_link *link)
{
	int	x1;
	int	y1;
	int	x2;
	int	y2;

	logical_to_pixel(link->a->x, link->a->y, &visu->camera, &x1, &y1);
	logical_to_pixel(link->b->x, link->b->y, &visu->camera, &x2, &y2);
	thickLineColor(renderer, x1, y1, x2, y2, 2, COLOR_LINK);
}

static void	draw_one_room(SDL_Renderer *renderer, t_visu *visu, t_room *room)
{
	int			x;
	int			y;
	int			radius;
	uint32_t	color;

	color = get_room_color(visu, room);
	if (room == visu->farm->start || room == visu->farm->end)
	{
		draw_terminal_room(renderer, visu, room, color);
		return ;
	}
	logical_to_pixel(room->x, room->y, &visu->camera, &x, &y);
	radius = logical_radius_to_pixel(0.16, &visu->camera);
	filledCircleColor(renderer, x, y, radius, color);
	aacircleColor(renderer, x, y, radius, COLOR_ROOM_BORDER);
	if (visu->settings.show_room_names)
		stringColor(renderer, x + radius + 4, y - 4, room->name,
			COLOR_ROOM_BORDER);
}

static void	draw_terminal_room(SDL_Renderer *renderer, t_visu *visu,
	t_room *room, uint32_t color)
{
	int	x;
	int	y;
	int	w;
	int	h;

	logical_to_pixel(room->x, room->y, &visu->camera, &x, &y);
	w = 82;
	h = 38;
	boxColor(renderer, x - w / 2, y - h / 2, x + w / 2, y + h / 2, color);
	rectangleColor(renderer, x - w / 2, y - h / 2, x + w / 2,
		y + h / 2, COLOR_ROOM_BORDER);
	if (visu->settings.show_room_names)
		stringColor(renderer, x - w / 2 + 6, y - 14, room->name,
			COLOR_TEXT);
}

static void	draw_hud(SDL_Renderer *renderer, t_visu *visu)
{
	char	buffer[128];

	snprintf(buffer, sizeof(buffer), "slide %d / %d",
		visu->anim.current_slide, visu->anim.step_count);
	stringColor(renderer, 12, 12, buffer, COLOR_TEXT);
}

static void	draw_controls(SDL_Renderer *renderer, t_visu *visu)
{
	(void)visu;
	boxColor(renderer, 8, 30, 240, 112, COLOR_PANEL_BG);
	rectangleColor(renderer, 8, 30, 240, 112, COLOR_PANEL_BORDER);
	stringColor(renderer, 18, 42, "left/right : prev/next", COLOR_TEXT);
	stringColor(renderer, 18, 58, "N : room names", COLOR_TEXT);
	stringColor(renderer, 18, 74, "A : ant ids", COLOR_TEXT);
	stringColor(renderer, 18, 90, "L : links | H : hud | TAB : panel",
		COLOR_TEXT);
}

static uint32_t	get_room_color(t_visu *visu, t_room *room)
{
	if (room == visu->farm->start)
		return (COLOR_START);
	if (room == visu->farm->end)
		return (COLOR_END);
	return (COLOR_ROOM);
}