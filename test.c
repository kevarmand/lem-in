#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>

#define WIN_W 900
#define WIN_H 500

static int	init(SDL_Window **window, SDL_Renderer **renderer);
static void	draw_test(SDL_Renderer *renderer);
static void	draw_case(SDL_Renderer *renderer, int x, int y, Uint32 color,
				const char *label);
static int	loop(SDL_Renderer *renderer);

int	main(void)
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	int				ret;

	window = NULL;
	renderer = NULL;
	ret = init(&window, &renderer);
	if (ret)
		return (ret);
	ret = loop(renderer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (ret);
}

static int	init(SDL_Window **window, SDL_Renderer **renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return (1);
	}
	*window = SDL_CreateWindow("SDL2_gfx color test",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (!*window)
	{
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
	if (!*renderer)
	{
		fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(*window);
		SDL_Quit();
		return (1);
	}
	return (0);
}

static int	loop(SDL_Renderer *renderer)
{
	SDL_Event	event;
	int			running;

	running = 1;
	draw_test(renderer);
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = 0;
			if (event.type == SDL_KEYDOWN
				&& event.key.keysym.sym == SDLK_ESCAPE)
				running = 0;
		}
		SDL_Delay(10);
	}
	return (0);
}

static void	draw_test(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
	SDL_RenderClear(renderer);

	thickLineRGBA(renderer, 0, 250, WIN_W, 250, 8, 255, 0, 0, 255);
	thickLineRGBA(renderer, 450, 0, 450, WIN_H, 8, 255, 0, 0, 255);

	draw_case(renderer, 120, 120, 0xFF0000FF, "Color 0xFF0000FF");
	draw_case(renderer, 320, 120, 0x0000FFFF, "Color 0x0000FFFF");
	draw_case(renderer, 520, 120, 0xFFFF0000, "Color 0xFFFF0000");
	draw_case(renderer, 720, 120, 0xFF00FFFF, "Color 0xFF00FFFF");

	filledCircleRGBA(renderer, 120, 360, 60, 255, 0, 0, 255);
	stringRGBA(renderer, 60, 430, "RGBA red alpha 255", 255, 255, 255, 255);

	filledCircleRGBA(renderer, 320, 360, 60, 0, 255, 0, 255);
	stringRGBA(renderer, 260, 430, "RGBA green alpha 255", 255, 255, 255, 255);

	filledCircleRGBA(renderer, 520, 360, 60, 0, 0, 255, 255);
	stringRGBA(renderer, 465, 430, "RGBA blue alpha 255", 255, 255, 255, 255);

	stringRGBA(renderer, 20, 20, "ESC to quit", 255, 255, 255, 255);
	SDL_RenderPresent(renderer);
}

static void	draw_case(SDL_Renderer *renderer, int x, int y, Uint32 color,
	const char *label)
{
	filledCircleColor(renderer, x, y, 60, color);
	aacircleColor(renderer, x, y, 60, 0xFFFFFFFF);
	stringColor(renderer, x - 65, y + 80, label, 0xFFFFFFFF);
}