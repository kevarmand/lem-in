#include <math.h>
#include "visualizer.h"
#include "error.h"

static SDL_Texture	*background_create_circle_texture(SDL_Renderer *renderer,
						int radius, int border);
static SDL_Surface	*background_create_circle_surface(int radius, int border);
static int			background_fill_circle_surface(SDL_Surface *surface,
						int radius, int border);
static void			background_set_circle_pixel(SDL_Surface *surface, int x,
						int y, Uint8 alpha);
static Uint8		background_circle_alpha(double distance, double radius,
						int border);

int	background_init_circle_cache(SDL_Renderer *renderer, t_visu *visu)
{
	int	radius;

	radius = 0;
	while (radius <= ROOM_RADIUS_MAX)
	{
		visu->background.circle_fill[radius] = NULL;
		visu->background.circle_border[radius] = NULL;
		radius++;
	}
	radius = 1;
	while (radius <= ROOM_RADIUS_MAX)
	{
		visu->background.circle_fill[radius]
			= background_create_circle_texture(renderer, radius, 0);
		if (!visu->background.circle_fill[radius])
			return (ERR_UNKNOWN);
		visu->background.circle_border[radius]
			= background_create_circle_texture(renderer, radius, 1);
		if (!visu->background.circle_border[radius])
			return (ERR_UNKNOWN);
		radius++;
	}
	return (ERR_NO_ERROR);
}

void	background_destroy_circle_cache(t_visu *visu)
{
	int	radius;

	radius = 0;
	while (radius <= ROOM_RADIUS_MAX)
	{
		if (visu->background.circle_fill[radius])
			SDL_DestroyTexture(visu->background.circle_fill[radius]);
		if (visu->background.circle_border[radius])
			SDL_DestroyTexture(visu->background.circle_border[radius]);
		visu->background.circle_fill[radius] = NULL;
		visu->background.circle_border[radius] = NULL;
		radius++;
	}
}

static SDL_Texture	*background_create_circle_texture(SDL_Renderer *renderer,
	int radius, int border)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;

	surface = background_create_circle_surface(radius, border);
	if (!surface)
		return (NULL);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture)
		return (NULL);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return (texture);
}

static SDL_Surface	*background_create_circle_surface(int radius, int border)
{
	SDL_Surface	*surface;
	int			size;

	size = radius * 2 + 3;
	surface = SDL_CreateRGBSurfaceWithFormat(0, size, size, 32,
			SDL_PIXELFORMAT_RGBA8888);
	if (!surface)
		return (NULL);
	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 255,
			255, 0));
	if (background_fill_circle_surface(surface, radius, border))
	{
		SDL_FreeSurface(surface);
		return (NULL);
	}
	return (surface);
}

static int	background_fill_circle_surface(SDL_Surface *surface, int radius,
	int border)
{
	double	center;
	double	dx;
	double	dy;
	int		x;
	int		y;

	if (SDL_LockSurface(surface) < 0)
		return (ERR_UNKNOWN);
	center = (double)radius + 1.0;
	y = 0;
	while (y < surface->h)
	{
		x = 0;
		while (x < surface->w)
		{
			dx = (double)x - center;
			dy = (double)y - center;
			background_set_circle_pixel(surface, x, y,
				background_circle_alpha(sqrt(dx * dx + dy * dy),
					(double)radius, border));
			x++;
		}
		y++;
	}
	SDL_UnlockSurface(surface);
	return (ERR_NO_ERROR);
}

static void	background_set_circle_pixel(SDL_Surface *surface, int x, int y,
	Uint8 alpha)
{
	Uint32	*row;

	row = (Uint32 *)((char *)surface->pixels + y * surface->pitch);
	row[x] = SDL_MapRGBA(surface->format, 255, 255, 255, alpha);
}

static Uint8	background_circle_alpha(double distance, double radius,
	int border)
{
	double	diff;

	if (!border)
	{
		if (distance <= radius - 0.5)
			return (255);
		if (distance <= radius + 0.5)
			return ((Uint8)((radius + 0.5 - distance) * 255.0));
		return (0);
	}
	diff = fabs(distance - radius);
	if (diff >= 1.0)
		return (0);
	return ((Uint8)((1.0 - diff) * 255.0));
}