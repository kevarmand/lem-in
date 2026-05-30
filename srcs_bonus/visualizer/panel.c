#include <stdio.h>
#include "visualizer.h"

static int	panel_key_event(SDL_Event *event, t_visu *visu, int *need_redraw);
static int	panel_mouse_event(SDL_Event *event, t_visu *visu, int *need_redraw);
static int	panel_build_buttons(t_visu *visu, t_panel_button *buttons);
static void	panel_draw_box(SDL_Renderer *renderer);
static void	panel_draw_stats(SDL_Renderer *renderer, t_visu *visu);
static void	panel_draw_buttons(SDL_Renderer *renderer, t_visu *visu);
static void	panel_draw_button(SDL_Renderer *renderer, t_visu *visu,
				t_panel_button *button);
static void	panel_draw_option_label(SDL_Renderer *renderer, t_visu *visu,
				t_panel_button *button);
static int	panel_option_value(t_visu *visu, int option_id);
static int	panel_button_contains(t_panel_button *button, int x, int y);
static int	panel_moving_count(t_visu *visu);
static void	panel_set_color(SDL_Renderer *renderer, uint32_t color);

void	panel_draw(SDL_Renderer *renderer, t_visu *visu)
{
	if (!visu->settings.show_hud && !visu->settings.show_panel)
		return ;
	panel_draw_box(renderer);
	if (visu->settings.show_hud)
		panel_draw_stats(renderer, visu);
	if (visu->settings.show_panel)
		panel_draw_buttons(renderer, visu);
}

int	panel_handle_event(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	if (event->type == SDL_KEYDOWN)
		return (panel_key_event(event, visu, need_redraw));
	if (event->type == SDL_MOUSEBUTTONDOWN)
		return (panel_mouse_event(event, visu, need_redraw));
	return (0);
}

void	panel_toggle_option(t_visu *visu, int option_id, int *need_redraw)
{
	if (option_id == PANEL_OPTION_LINKS)
	{
		visu->settings.show_links = !visu->settings.show_links;
		background_invalidate(visu);
	}
	else if (option_id == PANEL_OPTION_UNUSED_LINKS)
	{
		visu->settings.show_unused_links = !visu->settings.show_unused_links;
		background_invalidate(visu);
	}
	else if (option_id == PANEL_OPTION_UNUSED_ROOMS)
	{
		visu->settings.show_unused_rooms = !visu->settings.show_unused_rooms;
		background_invalidate(visu);
	}
	else if (option_id == PANEL_OPTION_PATH_COLORS)
	{
		visu->settings.color_paths = !visu->settings.color_paths;
		background_invalidate(visu);
	}
	else if (option_id == PANEL_OPTION_ROOM_NAMES)
	{
		visu->settings.show_room_names = !visu->settings.show_room_names;
		background_invalidate(visu);
	}
	else if (option_id == PANEL_OPTION_ANT_IDS)
		visu->settings.show_ant_ids = !visu->settings.show_ant_ids;
	else if (option_id == PANEL_OPTION_HUD)
		visu->settings.show_hud = !visu->settings.show_hud;
	else if (option_id == PANEL_OPTION_PANEL)
		visu->settings.show_panel = !visu->settings.show_panel;
	*need_redraw = 1;
}

static int	panel_key_event(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	if (event->key.keysym.sym == SDLK_l)
		panel_toggle_option(visu, PANEL_OPTION_LINKS, need_redraw);
	else if (event->key.keysym.sym == SDLK_u)
		panel_toggle_option(visu, PANEL_OPTION_UNUSED_LINKS, need_redraw);
	else if (event->key.keysym.sym == SDLK_e)
		panel_toggle_option(visu, PANEL_OPTION_UNUSED_ROOMS, need_redraw);
	else if (event->key.keysym.sym == SDLK_c)
		panel_toggle_option(visu, PANEL_OPTION_PATH_COLORS, need_redraw);
	else if (event->key.keysym.sym == SDLK_n)
		panel_toggle_option(visu, PANEL_OPTION_ROOM_NAMES, need_redraw);
	else if (event->key.keysym.sym == SDLK_a)
		panel_toggle_option(visu, PANEL_OPTION_ANT_IDS, need_redraw);
	else if (event->key.keysym.sym == SDLK_h)
		panel_toggle_option(visu, PANEL_OPTION_HUD, need_redraw);
	else if (event->key.keysym.sym == SDLK_TAB)
		panel_toggle_option(visu, PANEL_OPTION_PANEL, need_redraw);
	else
		return (0);
	return (1);
}

static int	panel_mouse_event(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	t_panel_button	buttons[16];
	int				count;
	int				i;

	if (!visu->settings.show_panel)
		return (0);
	if (event->button.button != SDL_BUTTON_LEFT)
		return (0);
	count = panel_build_buttons(visu, buttons);
	i = 0;
	while (i < count)
	{
		if (panel_button_contains(&buttons[i], event->button.x,
				event->button.y))
		{
			panel_toggle_option(visu, buttons[i].option_id, need_redraw);
			return (1);
		}
		i++;
	}
	if (event->button.x >= 8 && event->button.x <= 300
		&& event->button.y >= 30 && event->button.y <= 292)
		return (1);
	return (0);
}

static int	panel_build_buttons(t_visu *visu, t_panel_button *buttons)
{
	int	y;
	int	i;

	(void)visu;
	y = 154;
	i = 0;
	buttons[i++] = (t_panel_button){{18, y, 122, 18}, PANEL_OPTION_LINKS,
		"Links"};
	y += 20;
	buttons[i++] = (t_panel_button){{18, y, 122, 18},
		PANEL_OPTION_UNUSED_LINKS, "Unused links"};
	y += 20;
	buttons[i++] = (t_panel_button){{18, y, 122, 18},
		PANEL_OPTION_UNUSED_ROOMS, "Empty rooms"};
	y += 20;
	buttons[i++] = (t_panel_button){{18, y, 122, 18},
		PANEL_OPTION_PATH_COLORS, "Path colors"};
	y += 20;
	buttons[i++] = (t_panel_button){{18, y, 122, 18},
		PANEL_OPTION_ROOM_NAMES, "Room names"};
	y += 20;
	buttons[i++] = (t_panel_button){{18, y, 122, 18},
		PANEL_OPTION_ANT_IDS, "Ant ids"};
	y += 20;
	buttons[i++] = (t_panel_button){{18, y, 122, 18},
		PANEL_OPTION_HUD, "Stats"};
	return (i);
}

static void	panel_draw_box(SDL_Renderer *renderer)
{
	boxColor(renderer, 8, 30, 300, 292, COLOR_PANEL_BG);
	rectangleColor(renderer, 8, 30, 300, 292, COLOR_PANEL_BORDER);
	stringColor(renderer, 18, 42, "LEM-IN VISUALIZER", COLOR_TEXT);
}

static void	panel_draw_stats(SDL_Renderer *renderer, t_visu *visu)
{
	char	buffer[128];

	snprintf(buffer, sizeof(buffer), "slide      %d / %d",
		visu->anim.current_slide, visu->anim.step_count);
	stringColor(renderer, 18, 62, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "state      %s",
		visu->anim.playing ? "PLAY" : "PAUSE");
	stringColor(renderer, 18, 78, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "ants       %d", visu->farm->ants);
	stringColor(renderer, 18, 94, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "start      %d", visu->anim.start_count);
	stringColor(renderer, 18, 110, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "active     %d", visu->anim.active_count);
	stringColor(renderer, 18, 126, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "end        %d", visu->anim.end_count);
	stringColor(renderer, 150, 110, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "moving     %d", panel_moving_count(visu));
	stringColor(renderer, 150, 126, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "visible    L:%d R:%d",
		visu->background.line_count, visu->background.circle_count);
	stringColor(renderer, 18, 142, buffer, COLOR_TEXT);
}

static void	panel_draw_buttons(SDL_Renderer *renderer, t_visu *visu)
{
	t_panel_button	buttons[16];
	int				count;
	int				i;

	count = panel_build_buttons(visu, buttons);
	i = 0;
	while (i < count)
	{
		panel_draw_button(renderer, visu, &buttons[i]);
		i++;
	}
	stringColor(renderer, 18, 274, "TAB panel | SPACE play", COLOR_TEXT);
}

static void	panel_draw_button(SDL_Renderer *renderer, t_visu *visu,
	t_panel_button *button)
{
	uint32_t	color;

	if (panel_option_value(visu, button->option_id))
		color = GFX_COLOR(45, 90, 70, 255);
	else
		color = GFX_COLOR(60, 60, 60, 255);
	boxColor(renderer, button->rect.x, button->rect.y,
		button->rect.x + button->rect.w, button->rect.y + button->rect.h,
		color);
	rectangleColor(renderer, button->rect.x, button->rect.y,
		button->rect.x + button->rect.w, button->rect.y + button->rect.h,
		COLOR_PANEL_BORDER);
	panel_draw_option_label(renderer, visu, button);
}

static void	panel_draw_option_label(SDL_Renderer *renderer, t_visu *visu,
	t_panel_button *button)
{
	char	buffer[64];

	if (panel_option_value(visu, button->option_id))
		snprintf(buffer, sizeof(buffer), "[ON ] %s", button->label);
	else
		snprintf(buffer, sizeof(buffer), "[OFF] %s", button->label);
	stringColor(renderer, button->rect.x + 6, button->rect.y + 5, buffer,
		COLOR_TEXT);
}

static int	panel_option_value(t_visu *visu, int option_id)
{
	if (option_id == PANEL_OPTION_LINKS)
		return (visu->settings.show_links);
	if (option_id == PANEL_OPTION_UNUSED_LINKS)
		return (visu->settings.show_unused_links);
	if (option_id == PANEL_OPTION_UNUSED_ROOMS)
		return (visu->settings.show_unused_rooms);
	if (option_id == PANEL_OPTION_PATH_COLORS)
		return (visu->settings.color_paths);
	if (option_id == PANEL_OPTION_ROOM_NAMES)
		return (visu->settings.show_room_names);
	if (option_id == PANEL_OPTION_ANT_IDS)
		return (visu->settings.show_ant_ids);
	if (option_id == PANEL_OPTION_HUD)
		return (visu->settings.show_hud);
	if (option_id == PANEL_OPTION_PANEL)
		return (visu->settings.show_panel);
	return (0);
}

static int	panel_button_contains(t_panel_button *button, int x, int y)
{
	if (x < button->rect.x)
		return (0);
	if (x > button->rect.x + button->rect.w)
		return (0);
	if (y < button->rect.y)
		return (0);
	if (y > button->rect.y + button->rect.h)
		return (0);
	return (1);
}

static int	panel_moving_count(t_visu *visu)
{
	t_step	*step;
	int		index;

	if (visu->anim.transition.active)
		index = visu->anim.transition.step_index;
	else
		index = visu->anim.current_slide;
	if (index < 0 || index >= visu->anim.step_count)
		return (0);
	step = (t_step *)visu->anim.steps.data[index];
	return ((int)step->moves.count);
}

static void	panel_set_color(SDL_Renderer *renderer, uint32_t color)
{
	SDL_SetRenderDrawColor(renderer, color & 0xFF, (color >> 8) & 0xFF,
		(color >> 16) & 0xFF, (color >> 24) & 0xFF);
}