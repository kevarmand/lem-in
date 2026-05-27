/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:09:54 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/27 16:43:51 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include "visualizer.h"
#include "utils.h"
#include "error.h"

static int			background_create_texture(SDL_Renderer *renderer,
						t_visu *visu);
static int			background_init_commands(t_visu *visu);
static int			background_init_marks(t_visu *visu);
static int			background_init_link_ids(t_visu *visu);
static int			background_init_circle_cache(SDL_Renderer *renderer,
						t_visu *visu);
static int			background_mark_solution(t_visu *visu);
static void			background_mark_move(t_visu *visu, t_move *move);
static int			background_link_slot(t_visu *visu, t_link *link);
static int			background_link_id(t_visu *visu, t_link *link);
static t_link		*background_move_link(t_visu *visu, t_move *move);
static SDL_Texture	*background_create_circle_texture(SDL_Renderer *renderer,
						int radius, int border);
static SDL_Surface	*background_create_circle_surface(int radius, int border);
static int			background_fill_circle_surface(SDL_Surface *surface,
						int radius, int border);
static void			background_set_circle_pixel(SDL_Surface *surface, int x,
						int y, Uint8 alpha);
static Uint8		background_circle_alpha(double distance, double radius,
						int border);
static void			background_prepare_commands(t_visu *visu);
static void			background_prepare_links(t_visu *visu);
static void			background_prepare_rooms(t_visu *visu);
static int			background_line_visible(t_visu *visu, t_line_cmd *cmd);
static int			background_circle_visible(t_visu *visu, t_circle_cmd *cmd);
static void			background_push_line(t_visu *visu, t_line_cmd *cmd);
static void			background_push_circle(t_visu *visu, t_circle_cmd *cmd);
static void			background_push_text(t_visu *visu, t_text_cmd *cmd);
static void			background_draw_commands(SDL_Renderer *renderer,
						t_visu *visu);
static void			background_draw_lines(SDL_Renderer *renderer, t_visu *visu);
static void			background_draw_circles(SDL_Renderer *renderer,
						t_visu *visu);
static void			background_draw_texts(SDL_Renderer *renderer, t_visu *visu);
static void			background_draw_circle_sprite(SDL_Renderer *renderer,
						t_visu *visu, t_circle_cmd *cmd);
static void			background_set_texture_color(SDL_Texture *texture,
						uint32_t color);
static void			background_destroy_circle_cache(t_visu *visu);
static double		profile_elapsed_ms(Uint64 start, Uint64 end);

int	background_init(SDL_Renderer *renderer, t_visu *visu)
{
	int	err;

	err = background_init_commands(visu);
	if (err)
		return (err);
	err = background_init_marks(visu);
	if (err)
		return (err);
	err = background_init_link_ids(visu);
	if (err)
		return (err);
	err = background_mark_solution(visu);
	if (err)
		return (err);
	err = background_init_circle_cache(renderer, visu);
	if (err)
		return (err);
	return (ERR_NO_ERROR);
}

void	background_invalidate(t_visu *visu)
{
	visu->background.dirty = 1;
}

void	background_destroy(t_visu *visu)
{
	if (visu->background.texture)
		SDL_DestroyTexture(visu->background.texture);
	background_destroy_circle_cache(visu);
	free(visu->background.lines);
	free(visu->background.circles);
	free(visu->background.texts);
	free(visu->background.room_used);
	free(visu->background.room_color);
	free(visu->background.link_used);
	free(visu->background.link_color);
	free(visu->background.link_ids);
	visu->background.texture = NULL;
	visu->background.lines = NULL;
	visu->background.circles = NULL;
	visu->background.texts = NULL;
	visu->background.room_used = NULL;
	visu->background.room_color = NULL;
	visu->background.link_used = NULL;
	visu->background.link_color = NULL;
	visu->background.link_ids = NULL;
	visu->background.dirty = 1;
	visu->background.width = 0;
	visu->background.height = 0;
	visu->background.line_count = 0;
	visu->background.circle_count = 0;
	visu->background.text_count = 0;
	visu->background.line_capacity = 0;
	visu->background.circle_capacity = 0;
	visu->background.text_capacity = 0;
	visu->background.link_id_capacity = 0;
}

int	background_rebuild(SDL_Renderer *renderer, t_visu *visu)
{
	SDL_Texture	*old_target;
	Uint64		start;
	Uint64		static_start;
	Uint64		end;

	start = SDL_GetPerformanceCounter();
	if (!visu->background.texture
		|| visu->background.width != visu->camera.win_width
		|| visu->background.height != visu->camera.win_height)
	{
		if (background_create_texture(renderer, visu))
			return (ERR_UNKNOWN);
	}
	background_prepare_commands(visu);
	old_target = SDL_GetRenderTarget(renderer);
	if (SDL_SetRenderTarget(renderer, visu->background.texture) < 0)
		return (ERR_UNKNOWN);
	SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
	SDL_RenderClear(renderer);
	static_start = SDL_GetPerformanceCounter();
	background_draw_commands(renderer, visu);
	end = SDL_GetPerformanceCounter();
	if (SDL_SetRenderTarget(renderer, old_target) < 0)
		return (ERR_UNKNOWN);
	visu->profile.static_map_ms = profile_elapsed_ms(static_start, end)
		+ visu->profile.prepare_ms;
	visu->profile.links_ms = visu->profile.prepare_links_ms
		+ visu->profile.draw_links_ms;
	visu->profile.rooms_ms = visu->profile.prepare_rooms_ms
		+ visu->profile.draw_rooms_ms;
	visu->background.dirty = 0;
	end = SDL_GetPerformanceCounter();
	visu->profile.background_ms = profile_elapsed_ms(start, end);
	return (ERR_NO_ERROR);
}

void	background_render(SDL_Renderer *renderer, t_visu *visu)
{
	SDL_RenderCopy(renderer, visu->background.texture, NULL, NULL);
}

static int	background_init_commands(t_visu *visu)
{
	visu->background.line_capacity = (int)visu->farm->links.count;
	visu->background.circle_capacity = (int)visu->farm->rooms.count;
	visu->background.text_capacity = (int)visu->farm->rooms.count;
	visu->background.lines = malloc(sizeof(*visu->background.lines)
			* visu->background.line_capacity);
	if (!visu->background.lines)
		return (ERR_MALLOC);
	visu->background.circles = malloc(sizeof(*visu->background.circles)
			* visu->background.circle_capacity);
	if (!visu->background.circles)
		return (ERR_MALLOC);
	visu->background.texts = malloc(sizeof(*visu->background.texts)
			* visu->background.text_capacity);
	if (!visu->background.texts)
		return (ERR_MALLOC);
	visu->background.line_count = 0;
	visu->background.circle_count = 0;
	visu->background.text_count = 0;
	return (ERR_NO_ERROR);
}

static int	background_init_marks(t_visu *visu)
{
	size_t	i;

	visu->background.room_used = malloc(sizeof(int) * visu->farm->rooms.count);
	if (!visu->background.room_used)
		return (ERR_MALLOC);
	visu->background.room_color = malloc(sizeof(uint32_t)
			* visu->farm->rooms.count);
	if (!visu->background.room_color)
		return (ERR_MALLOC);
	visu->background.link_used = malloc(sizeof(int) * visu->farm->links.count);
	if (!visu->background.link_used)
		return (ERR_MALLOC);
	visu->background.link_color = malloc(sizeof(uint32_t)
			* visu->farm->links.count);
	if (!visu->background.link_color)
		return (ERR_MALLOC);
	i = 0;
	while (i < visu->farm->rooms.count)
	{
		visu->background.room_used[i] = 0;
		visu->background.room_color[i] = COLOR_ROOM_BORDER;
		i++;
	}
	i = 0;
	while (i < visu->farm->links.count)
	{
		visu->background.link_used[i] = 0;
		visu->background.link_color[i] = COLOR_LINK;
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	background_init_link_ids(t_visu *visu)
{
	t_link	*link;
	int		i;
	int		slot;

	visu->background.link_id_capacity = (int)visu->farm->links.count * 2 + 1;
	visu->background.link_ids = malloc(sizeof(*visu->background.link_ids)
			* visu->background.link_id_capacity);
	if (!visu->background.link_ids)
		return (ERR_MALLOC);
	i = 0;
	while (i < visu->background.link_id_capacity)
	{
		visu->background.link_ids[i].link = NULL;
		visu->background.link_ids[i].id = -1;
		i++;
	}
	i = 0;
	while (i < (int)visu->farm->links.count)
	{
		link = (t_link *)visu->farm->links.data[i];
		slot = background_link_slot(visu, link);
		visu->background.link_ids[slot].link = link;
		visu->background.link_ids[slot].id = i;
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	background_mark_solution(t_visu *visu)
{
	t_step	*step;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < visu->anim.steps.count)
	{
		step = (t_step *)visu->anim.steps.data[i];
		j = 0;
		while (j < step->moves.count)
		{
			background_mark_move(visu, (t_move *)step->moves.data[j]);
			j++;
		}
		i++;
	}
	return (ERR_NO_ERROR);
}

static void	background_mark_move(t_visu *visu, t_move *move)
{
	t_link		*link;
	uint32_t	color;
	int			link_id;

	color = visu_ant_path_color(visu, move->path_id, 0);
	visu->background.room_used[move->from->id] = 1;
	visu->background.room_color[move->from->id] = color;
	visu->background.room_used[move->to->id] = 1;
	visu->background.room_color[move->to->id] = color;
	link = background_move_link(visu, move);
	if (!link)
		return ;
	link_id = background_link_id(visu, link);
	if (link_id < 0)
		return ;
	visu->background.link_used[link_id] = 1;
	visu->background.link_color[link_id] = color;
}

static int	background_link_slot(t_visu *visu, t_link *link)
{
	uintptr_t	hash;
	int			slot;

	hash = (uintptr_t)link;
	slot = (int)((hash >> 4) % (uintptr_t)visu->background.link_id_capacity);
	while (visu->background.link_ids[slot].link
		&& visu->background.link_ids[slot].link != link)
	{
		slot++;
		if (slot == visu->background.link_id_capacity)
			slot = 0;
	}
	return (slot);
}

static int	background_link_id(t_visu *visu, t_link *link)
{
	int	slot;

	slot = background_link_slot(visu, link);
	if (!visu->background.link_ids[slot].link)
		return (-1);
	return (visu->background.link_ids[slot].id);
}

static t_link	*background_move_link(t_visu *visu, t_move *move)
{
	t_link	*link;
	char	*key;

	key = make_link_key(move->from->name, move->to->name);
	if (!key)
		return (NULL);
	link = hashmap_get(visu->farm->links_by_key, key);
	free(key);
	return (link);
}

static int	background_init_circle_cache(SDL_Renderer *renderer, t_visu *visu)
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

static int	background_create_texture(SDL_Renderer *renderer, t_visu *visu)
{
	SDL_Texture	*texture;

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, visu->camera.win_width,
			visu->camera.win_height);
	if (!texture)
		return (ERR_UNKNOWN);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
	if (visu->background.texture)
		SDL_DestroyTexture(visu->background.texture);
	visu->background.texture = texture;
	visu->background.width = visu->camera.win_width;
	visu->background.height = visu->camera.win_height;
	visu->background.dirty = 1;
	return (ERR_NO_ERROR);
}

static void	background_prepare_commands(t_visu *visu)
{
	Uint64	start;
	Uint64	end;

	start = SDL_GetPerformanceCounter();
	visu->background.line_count = 0;
	visu->background.circle_count = 0;
	visu->background.text_count = 0;
	visu->profile.prepare_links_ms = 0.0;
	visu->profile.prepare_rooms_ms = 0.0;
	visu->profile.prepare_texts_ms = 0.0;
	visu->profile.draw_links_ms = 0.0;
	visu->profile.draw_rooms_ms = 0.0;
	visu->profile.draw_texts_ms = 0.0;
	if (visu->settings.show_links)
		background_prepare_links(visu);
	background_prepare_rooms(visu);
	end = SDL_GetPerformanceCounter();
	visu->profile.prepare_ms = profile_elapsed_ms(start, end);
}

static void	background_prepare_links(t_visu *visu)
{
	t_link		*link;
	t_line_cmd	cmd;
	size_t		i;
	Uint64		start;
	Uint64		end;

	start = SDL_GetPerformanceCounter();
	i = 0;
	while (i < visu->farm->links.count)
	{
		if (visu->settings.show_unused_links || visu->background.link_used[i])
		{
			link = (t_link *)visu->farm->links.data[i];
			logical_to_pixel(link->a->x, link->a->y, &visu->camera,
				&cmd.x1, &cmd.y1);
			logical_to_pixel(link->b->x, link->b->y, &visu->camera,
				&cmd.x2, &cmd.y2);
			cmd.thickness = 2;
			cmd.color = visu->background.link_color[i];
			if (background_line_visible(visu, &cmd))
				background_push_line(visu, &cmd);
		}
		i++;
	}
	end = SDL_GetPerformanceCounter();
	visu->profile.prepare_links_ms = profile_elapsed_ms(start, end);
}

static void	background_prepare_rooms(t_visu *visu)
{
	t_room			*room;
	t_circle_cmd	circle;
	t_text_cmd		text;
	size_t			i;
	int				radius;
	Uint64			start;
	Uint64			end;

	start = SDL_GetPerformanceCounter();
	radius = visu_room_radius(visu);
	i = 0;
	while (i < visu->farm->rooms.count)
	{
		room = (t_room *)visu->farm->rooms.data[i];
		if (room != visu->farm->start && room != visu->farm->end)
		{
			logical_to_pixel(room->x, room->y, &visu->camera,
				&circle.x, &circle.y);
			circle.radius = radius;
			circle.fill_color = COLOR_ROOM;
			circle.border_color = visu->background.room_color[room->id];
			circle.has_border = 1;
			if (background_circle_visible(visu, &circle))
			{
				background_push_circle(visu, &circle);
				if (visu->settings.show_room_names)
				{
					text.x = circle.x + radius + 4;
					text.y = circle.y - 4;
					text.text = room->name;
					text.color = circle.border_color;
					background_push_text(visu, &text);
				}
			}
		}
		i++;
	}
	end = SDL_GetPerformanceCounter();
	visu->profile.prepare_rooms_ms = profile_elapsed_ms(start, end);
}

static int	background_line_visible(t_visu *visu, t_line_cmd *cmd)
{
	int	pad;
	int	max_x;
	int	max_y;

	pad = cmd->thickness + 2;
	max_x = visu->camera.win_width + pad;
	max_y = visu->camera.win_height + pad;
	if (cmd->x1 < -pad && cmd->x2 < -pad)
		return (0);
	if (cmd->x1 > max_x && cmd->x2 > max_x)
		return (0);
	if (cmd->y1 < -pad && cmd->y2 < -pad)
		return (0);
	if (cmd->y1 > max_y && cmd->y2 > max_y)
		return (0);
	return (1);
}

static int	background_circle_visible(t_visu *visu, t_circle_cmd *cmd)
{
	int	pad;

	pad = cmd->radius + 2;
	if (cmd->x < -pad)
		return (0);
	if (cmd->x > visu->camera.win_width + pad)
		return (0);
	if (cmd->y < -pad)
		return (0);
	if (cmd->y > visu->camera.win_height + pad)
		return (0);
	return (1);
}

static void	background_push_line(t_visu *visu, t_line_cmd *cmd)
{
	int	i;

	i = visu->background.line_count;
	visu->background.lines[i] = *cmd;
	visu->background.line_count++;
}

static void	background_push_circle(t_visu *visu, t_circle_cmd *cmd)
{
	int	i;

	i = visu->background.circle_count;
	visu->background.circles[i] = *cmd;
	visu->background.circle_count++;
}

static void	background_push_text(t_visu *visu, t_text_cmd *cmd)
{
	int	i;

	i = visu->background.text_count;
	visu->background.texts[i] = *cmd;
	visu->background.text_count++;
}

static void	background_draw_commands(SDL_Renderer *renderer, t_visu *visu)
{
	background_draw_lines(renderer, visu);
	background_draw_circles(renderer, visu);
	background_draw_texts(renderer, visu);
}

static void	background_draw_lines(SDL_Renderer *renderer, t_visu *visu)
{
	t_line_cmd	*cmd;
	int			i;
	Uint64		start;
	Uint64		end;

	start = SDL_GetPerformanceCounter();
	i = 0;
	while (i < visu->background.line_count)
	{
		cmd = &visu->background.lines[i];
		thickLineColor(renderer, cmd->x1, cmd->y1, cmd->x2, cmd->y2,
			cmd->thickness, cmd->color);
		i++;
	}
	end = SDL_GetPerformanceCounter();
	visu->profile.draw_links_ms = profile_elapsed_ms(start, end);
}

static void	background_draw_circles(SDL_Renderer *renderer, t_visu *visu)
{
	t_circle_cmd	*cmd;
	int				i;
	Uint64			start;
	Uint64			end;

	start = SDL_GetPerformanceCounter();
	i = 0;
	while (i < visu->background.circle_count)
	{
		cmd = &visu->background.circles[i];
		background_draw_circle_sprite(renderer, visu, cmd);
		i++;
	}
	end = SDL_GetPerformanceCounter();
	visu->profile.draw_rooms_ms = profile_elapsed_ms(start, end);
}

static void	background_draw_circle_sprite(SDL_Renderer *renderer, t_visu *visu,
	t_circle_cmd *cmd)
{
	SDL_Rect	dst;
	int			size;

	if (cmd->radius <= 0 || cmd->radius > ROOM_RADIUS_MAX)
		return ;
	size = cmd->radius * 2 + 3;
	dst.x = cmd->x - cmd->radius - 1;
	dst.y = cmd->y - cmd->radius - 1;
	dst.w = size;
	dst.h = size;
	background_set_texture_color(visu->background.circle_fill[cmd->radius],
		cmd->fill_color);
	SDL_RenderCopy(renderer, visu->background.circle_fill[cmd->radius],
		NULL, &dst);
	if (cmd->has_border)
	{
		background_set_texture_color(
			visu->background.circle_border[cmd->radius],
			cmd->border_color);
		SDL_RenderCopy(renderer, visu->background.circle_border[cmd->radius],
			NULL, &dst);
	}
}

static void	background_draw_texts(SDL_Renderer *renderer, t_visu *visu)
{
	t_text_cmd	*cmd;
	int			i;
	Uint64		start;
	Uint64		end;

	start = SDL_GetPerformanceCounter();
	i = 0;
	while (i < visu->background.text_count)
	{
		cmd = &visu->background.texts[i];
		stringColor(renderer, cmd->x, cmd->y, cmd->text, cmd->color);
		i++;
	}
	end = SDL_GetPerformanceCounter();
	visu->profile.draw_texts_ms = profile_elapsed_ms(start, end);
}

static void	background_set_texture_color(SDL_Texture *texture, uint32_t color)
{
	SDL_SetTextureColorMod(texture, color & 0xFF, (color >> 8) & 0xFF,
		(color >> 16) & 0xFF);
	SDL_SetTextureAlphaMod(texture, (color >> 24) & 0xFF);
}

static void	background_destroy_circle_cache(t_visu *visu)
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

static double	profile_elapsed_ms(Uint64 start, Uint64 end)
{
	return ((double)(end - start) * 1000.0
		/ (double)SDL_GetPerformanceFrequency());
}