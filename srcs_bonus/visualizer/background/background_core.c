#include "visualizer.h"
#include "error.h"

static int	background_create_texture(SDL_Renderer *renderer, t_visu *visu);

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

double	profile_elapsed_ms(Uint64 start, Uint64 end)
{
	return ((double)(end - start) * 1000.0
		/ (double)SDL_GetPerformanceFrequency());
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