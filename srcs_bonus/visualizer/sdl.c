/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:37:12 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 13:26:28 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"
#include "error.h"

static int	init_sdl(SDL_Window **window, SDL_Renderer **renderer, t_visu *visu);
static void	cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer);
static int	main_loop(SDL_Renderer *renderer, t_visu *visu);

int	launch_visualizer(t_visu *visu)
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	int				err;

	window = NULL;
	renderer = NULL;
	err = init_sdl(&window, &renderer, visu);
	if (err)
		return (err);
	err = main_loop(renderer, visu);
	cleanup_sdl(window, renderer);
	return (err);
}

static int	init_sdl(SDL_Window **window, SDL_Renderer **renderer, t_visu *visu)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (ERR_UNKNOWN);
	*window = SDL_CreateWindow("lem-in visualizer", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);
	if (!*window)
	{
		SDL_Quit();
		return (ERR_UNKNOWN);
	}
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (!*renderer)
	{
		SDL_DestroyWindow(*window);
		SDL_Quit();
		return (ERR_UNKNOWN);
	}
	visu->camera.win_width = WINDOW_WIDTH;
	visu->camera.win_height = WINDOW_HEIGHT;
	camera_fit_farm(&visu->camera, visu->farm);
	return (ERR_NO_ERROR);
}

static int	main_loop(SDL_Renderer *renderer, t_visu *visu)
{
	int	running;
	int	need_redraw;

	running = 1;
	need_redraw = 1;
	while (running)
	{
		handle_events(visu, &running, &need_redraw);
		if (visu->anim.playing)
		{
			anim_update(visu);
			draw_scene(renderer, visu);
			need_redraw = 0;
		}
		else if (need_redraw)
		{
			draw_scene(renderer, visu);
			need_redraw = 0;
		}
		SDL_Delay(16);
	}
	return (ERR_NO_ERROR);
}

static void	cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}