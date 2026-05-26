/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:37:48 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 13:26:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"

static void	handle_key(SDL_Event *event, t_visu *visu, int *running,
				int *need_redraw);
static void	handle_wheel(SDL_Event *event, t_visu *visu, int *need_redraw);
static void	handle_drag(SDL_Event *event, t_visu *visu, int *need_redraw);

void	handle_events(t_visu *visu, int *running, int *need_redraw)
{
	SDL_Event	event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			*running = 0;
		else if (event.type == SDL_KEYDOWN)
			handle_key(&event, visu, running, need_redraw);
		else if (event.type == SDL_MOUSEWHEEL)
			handle_wheel(&event, visu, need_redraw);
		else
			handle_drag(&event, visu, need_redraw);
	}
}

static void	handle_key(SDL_Event *event, t_visu *visu, int *running,
	int *need_redraw)
{
	if (event->key.keysym.sym == SDLK_ESCAPE)
		*running = 0;
	else if (event->key.keysym.sym == SDLK_SPACE)
		anim_toggle_play(visu);
	else if (event->key.keysym.sym == SDLK_RIGHT
		&& !visu->anim.playing && !visu->anim.transition.active)
		timeline_next(visu);
	else if (event->key.keysym.sym == SDLK_LEFT
		&& !visu->anim.playing && !visu->anim.transition.active)
		timeline_prev(visu);
	else if (event->key.keysym.sym == SDLK_n)
		visu->settings.show_room_names = !visu->settings.show_room_names;
	else if (event->key.keysym.sym == SDLK_a)
		visu->settings.show_ant_ids = !visu->settings.show_ant_ids;
	else if (event->key.keysym.sym == SDLK_l)
		visu->settings.show_links = !visu->settings.show_links;
	else if (event->key.keysym.sym == SDLK_h)
		visu->settings.show_hud = !visu->settings.show_hud;
	else if (event->key.keysym.sym == SDLK_TAB)
		visu->settings.show_controls = !visu->settings.show_controls;
	*need_redraw = 1;
}

static void	handle_wheel(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	int	mouse_x;
	int	mouse_y;

	SDL_GetMouseState(&mouse_x, &mouse_y);
	if (event->wheel.y > 0)
		zoom_around_mouse(mouse_x, mouse_y, &visu->camera, 1.111111111);
	else if (event->wheel.y < 0)
		zoom_around_mouse(mouse_x, mouse_y, &visu->camera, 0.9);
	*need_redraw = 1;
}

static void	handle_drag(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	static int	is_dragging;
	static int	last_x;
	static int	last_y;
	int			dx;
	int			dy;

	if (event->type == SDL_MOUSEBUTTONDOWN
		&& event->button.button == SDL_BUTTON_LEFT)
	{
		is_dragging = 1;
		last_x = event->button.x;
		last_y = event->button.y;
	}
	else if (event->type == SDL_MOUSEBUTTONUP
		&& event->button.button == SDL_BUTTON_LEFT)
		is_dragging = 0;
	else if (event->type == SDL_MOUSEMOTION && is_dragging)
	{
		dx = event->motion.x - last_x;
		dy = event->motion.y - last_y;
		visu->camera.x_offset -= dx / visu->camera.zoom;
		visu->camera.y_offset += dy / visu->camera.zoom;
		last_x = event->motion.x;
		last_y = event->motion.y;
		*need_redraw = 1;
	}
}