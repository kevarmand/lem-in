#include "visualizer.h"

static void	background_draw_lines(SDL_Renderer *renderer, t_visu *visu);
static void	background_draw_circles(SDL_Renderer *renderer, t_visu *visu);
static void	background_draw_texts(SDL_Renderer *renderer, t_visu *visu);

void	background_draw_commands(SDL_Renderer *renderer, t_visu *visu)
{
	background_draw_lines(renderer, visu);
	background_draw_circles(renderer, visu);
	background_draw_texts(renderer, visu);
}

void	background_draw_circle_sprite(SDL_Renderer *renderer, t_visu *visu,
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

void	background_set_texture_color(SDL_Texture *texture, uint32_t color)
{
	SDL_SetTextureColorMod(texture, color & 0xFF, (color >> 8) & 0xFF,
		(color >> 16) & 0xFF);
	SDL_SetTextureAlphaMod(texture, (color >> 24) & 0xFF);
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