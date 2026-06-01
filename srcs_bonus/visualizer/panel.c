#include <stdio.h>
#include "visualizer.h"

#define PANEL_X 8
#define PANEL_Y 30
#define PANEL_W 376
#define PANEL_H 334

#define PANEL_PAD 10
#define PANEL_RIGHT (PANEL_X + PANEL_W)
#define PANEL_BOTTOM (PANEL_Y + PANEL_H)

#define BUTTON_W 172
#define BUTTON_H 18
#define BUTTON_GAP 8

static int		panel_key_event(SDL_Event *event, t_visu *visu,
					int *need_redraw);
static int		panel_mouse_event(SDL_Event *event, t_visu *visu,
					int *need_redraw);
static void		panel_run_action(t_visu *visu, int action_id,
					int *need_redraw);
static int		panel_build_buttons(t_panel_button *buttons);
static void		panel_set_button(t_panel_button *button, int x, int y,
					int action_id, const char *label);
static void		panel_draw_box(SDL_Renderer *renderer);
static void		panel_draw_header(SDL_Renderer *renderer, t_visu *visu);
static void		panel_draw_timeline(SDL_Renderer *renderer, t_visu *visu);
static void		panel_draw_timeline_bar(SDL_Renderer *renderer, t_visu *visu,
					int slide);
static void		panel_draw_ants_block(SDL_Renderer *renderer, t_visu *visu);
static void		panel_draw_ant_card(SDL_Renderer *renderer, int x, char *label,
					int value, int total);
static void		panel_draw_ant_bar(SDL_Renderer *renderer, int start,
					int active, int end, int total);
static void		panel_draw_view(SDL_Renderer *renderer, t_visu *visu);
static void		panel_draw_playback(SDL_Renderer *renderer, t_visu *visu);
static void		panel_draw_button(SDL_Renderer *renderer, t_visu *visu,
					t_panel_button *button);
static void		panel_draw_button_label(SDL_Renderer *renderer,
					t_panel_button *button, int value);
static int		panel_action_value(t_visu *visu, int action_id);
static int		panel_button_contains(t_panel_button *button, int x, int y);
static int		panel_display_slide(t_visu *visu);
static void		panel_get_ant_counts(t_visu *visu, int *start, int *active,
					int *end);
static void		panel_apply_room_count(t_visu *visu, t_room *room,
					int *counts, int delta);
static int		panel_percent(int value, int total);

void	panel_draw(SDL_Renderer *renderer, t_visu *visu)
{
	if (!visu->settings.show_overlay)
		return ;
	panel_draw_box(renderer);
	panel_draw_header(renderer, visu);
	panel_draw_timeline(renderer, visu);
	panel_draw_ants_block(renderer, visu);
	panel_draw_view(renderer, visu);
	panel_draw_playback(renderer, visu);
}

int	panel_handle_event(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	if (event->type == SDL_KEYDOWN)
		return (panel_key_event(event, visu, need_redraw));
	if (event->type == SDL_MOUSEBUTTONDOWN)
		return (panel_mouse_event(event, visu, need_redraw));
	return (0);
}

static int	panel_key_event(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	int	sym;

	sym = event->key.keysym.sym;
	if (sym == SDLK_TAB)
	{
		visu->settings.show_overlay = !visu->settings.show_overlay;
		*need_redraw = 1;
		return (1);
	}
	if (sym == SDLK_l)
		panel_run_action(visu, PANEL_ACTION_LINKS, need_redraw);
	else if (sym == SDLK_u)
		panel_run_action(visu, PANEL_ACTION_UNUSED_ELEMENTS, need_redraw);
	else if (sym == SDLK_c)
		panel_run_action(visu, PANEL_ACTION_PATH_COLORS, need_redraw);
	else if (sym == SDLK_n)
		panel_run_action(visu, PANEL_ACTION_ROOM_NAMES, need_redraw);
	else if (sym == SDLK_a)
		panel_run_action(visu, PANEL_ACTION_ANT_IDS, need_redraw);
	else if (sym == SDLK_PLUS || sym == SDLK_KP_PLUS || sym == SDLK_EQUALS)
		panel_run_action(visu, PANEL_ACTION_SPEED_UP, need_redraw);
	else if (sym == SDLK_MINUS || sym == SDLK_KP_MINUS)
		panel_run_action(visu, PANEL_ACTION_SPEED_DOWN, need_redraw);
	else
		return (0);
	return (1);
}

static int	panel_mouse_event(SDL_Event *event, t_visu *visu, int *need_redraw)
{
	t_panel_button	buttons[8];
	int				count;
	int				i;

	if (!visu->settings.show_overlay)
		return (0);
	if (event->button.button != SDL_BUTTON_LEFT)
		return (0);
	count = panel_build_buttons(buttons);
	i = 0;
	while (i < count)
	{
		if (panel_button_contains(&buttons[i], event->button.x,
				event->button.y))
		{
			panel_run_action(visu, buttons[i].action_id, need_redraw);
			return (1);
		}
		i++;
	}
	if (event->button.x >= PANEL_X && event->button.x <= PANEL_RIGHT
		&& event->button.y >= PANEL_Y && event->button.y <= PANEL_BOTTOM)
		return (1);
	return (0);
}

static void	panel_run_action(t_visu *visu, int action_id, int *need_redraw)
{
	if (action_id == PANEL_ACTION_LINKS)
	{
		visu->settings.show_links = !visu->settings.show_links;
		background_invalidate(visu);
	}
	else if (action_id == PANEL_ACTION_UNUSED_ELEMENTS)
	{
		visu->settings.show_unused_elements
			= !visu->settings.show_unused_elements;
		background_invalidate(visu);
	}
	else if (action_id == PANEL_ACTION_PATH_COLORS)
	{
		visu->settings.color_paths = !visu->settings.color_paths;
		background_invalidate(visu);
	}
	else if (action_id == PANEL_ACTION_ROOM_NAMES)
	{
		visu->settings.show_room_names = !visu->settings.show_room_names;
		background_invalidate(visu);
	}
	else if (action_id == PANEL_ACTION_ANT_IDS)
		visu->settings.show_ant_ids = !visu->settings.show_ant_ids;
	else if (action_id == PANEL_ACTION_SPEED_DOWN)
		anim_speed_down(visu);
	else if (action_id == PANEL_ACTION_SPEED_UP)
		anim_speed_up(visu);
	*need_redraw = 1;
}

static int	panel_build_buttons(t_panel_button *buttons)
{
	int	i;

	i = 0;
	panel_set_button(&buttons[i++], 18, 226, PANEL_ACTION_LINKS, "Links");
	panel_set_button(&buttons[i++], 204, 226, PANEL_ACTION_UNUSED_ELEMENTS,
		"Unused elements");
	panel_set_button(&buttons[i++], 18, 250, PANEL_ACTION_PATH_COLORS,
		"Path colors");
	panel_set_button(&buttons[i++], 204, 250, PANEL_ACTION_ROOM_NAMES,
		"Room names");
	panel_set_button(&buttons[i++], 18, 274, PANEL_ACTION_ANT_IDS,
		"Ant ids");
	panel_set_button(&buttons[i++], 18, 318, PANEL_ACTION_SPEED_DOWN, "-");
	panel_set_button(&buttons[i++], 314, 318, PANEL_ACTION_SPEED_UP, "+");
	return (i);
}

static void	panel_set_button(t_panel_button *button, int x, int y,
	int action_id, const char *label)
{
	button->rect.x = x;
	button->rect.y = y;
	button->rect.w = BUTTON_W;
	button->rect.h = BUTTON_H;
	button->action_id = action_id;
	button->label = label;
}

static void	panel_draw_box(SDL_Renderer *renderer)
{
	boxColor(renderer, PANEL_X, PANEL_Y, PANEL_RIGHT, PANEL_BOTTOM,
		COLOR_PANEL_BG);
	rectangleColor(renderer, PANEL_X, PANEL_Y, PANEL_RIGHT, PANEL_BOTTOM,
		COLOR_PANEL_BORDER);
}

static void	panel_draw_header(SDL_Renderer *renderer, t_visu *visu)
{
	char	buffer[64];

	stringColor(renderer, 18, 42, "LEM-IN VISUALIZER", COLOR_TEXT);
	if (visu->anim.playing)
		stringColor(renderer, 330, 42, "PLAY", COLOR_PANEL_END);
	else
		stringColor(renderer, 322, 42, "PAUSE", COLOR_PANEL_ACTIVE);
	snprintf(buffer, sizeof(buffer), "speed %s", anim_speed_label(visu));
	stringColor(renderer, 18, 58, buffer, COLOR_TEXT);
	hlineColor(renderer, 18, 374, 74, COLOR_PANEL_MUTED);
}

static void	panel_draw_timeline(SDL_Renderer *renderer, t_visu *visu)
{
	char	buffer[64];
	int		slide;

	slide = panel_display_slide(visu);
	snprintf(buffer, sizeof(buffer), "SLIDE %d / %d", slide,
		visu->anim.step_count);
	stringColor(renderer, 18, 86, buffer, COLOR_TEXT);
	panel_draw_timeline_bar(renderer, visu, slide);
	hlineColor(renderer, 18, 374, 110, COLOR_PANEL_MUTED);
}

static void	panel_draw_timeline_bar(SDL_Renderer *renderer, t_visu *visu,
	int slide)
{
	int	x1;
	int	x2;
	int	y1;
	int	y2;
	int	fill;

	x1 = 18;
	x2 = 374;
	y1 = 100;
	y2 = 106;
	fill = x1;
	if (visu->anim.step_count > 0)
		fill = x1 + ((x2 - x1) * slide) / visu->anim.step_count;
	boxColor(renderer, x1, y1, x2, y2, GFX_COLOR(45, 45, 45, 255));
	boxColor(renderer, x1, y1, fill, y2, GFX_COLOR(120, 120, 120, 255));
	rectangleColor(renderer, x1, y1, x2, y2, COLOR_PANEL_BORDER);
}

static void	panel_draw_ants_block(SDL_Renderer *renderer, t_visu *visu)
{
	char	buffer[64];
	int		start;
	int		active;
	int		end;

	panel_get_ant_counts(visu, &start, &active, &end);
	snprintf(buffer, sizeof(buffer), "ANTS %d", visu->farm->ants);
	stringColor(renderer, 18, 122, buffer, COLOR_TEXT);
	panel_draw_ant_card(renderer, 18, "START", start, visu->farm->ants);
	panel_draw_ant_card(renderer, 142, "ACTIVE", active, visu->farm->ants);
	panel_draw_ant_card(renderer, 266, "END", end, visu->farm->ants);
	panel_draw_ant_bar(renderer, start, active, end, visu->farm->ants);
	hlineColor(renderer, 18, 374, 210, COLOR_PANEL_MUTED);
}

static void	panel_draw_ant_card(SDL_Renderer *renderer, int x, char *label,
	int value, int total)
{
	char	buffer[32];
	uint32_t	color;

	color = COLOR_PANEL_START;
	if (label[0] == 'A')
		color = COLOR_PANEL_ACTIVE;
	else if (label[0] == 'E')
		color = COLOR_PANEL_END;
	boxColor(renderer, x, 140, x + 108, 190, color);
	rectangleColor(renderer, x, 140, x + 108, 190, COLOR_PANEL_BORDER);
	stringColor(renderer, x + 8, 148, label, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "%d", value);
	stringColor(renderer, x + 38, 163, buffer, COLOR_TEXT);
	snprintf(buffer, sizeof(buffer), "%d%%", panel_percent(value, total));
	stringColor(renderer, x + 38, 178, buffer, COLOR_TEXT);
}

static void	panel_draw_ant_bar(SDL_Renderer *renderer, int start, int active,
	int end, int total)
{
	int	x;
	int	y;
	int	w;
	int	start_w;
	int	active_w;

	x = 18;
	y = 198;
	w = 356;
	start_w = (w * start) / total;
	active_w = (w * active) / total;
	boxColor(renderer, x, y, x + start_w, y + 7, COLOR_PANEL_START);
	boxColor(renderer, x + start_w, y, x + start_w + active_w, y + 7,
		COLOR_PANEL_ACTIVE);
	boxColor(renderer, x + start_w + active_w, y, x + w, y + 7,
		COLOR_PANEL_END);
	rectangleColor(renderer, x, y, x + w, y + 7, COLOR_PANEL_BORDER);
	(void)end;
}

static void	panel_draw_view(SDL_Renderer *renderer, t_visu *visu)
{
	t_panel_button	buttons[8];
	int				i;

	panel_build_buttons(buttons);
	stringColor(renderer, 18, 218, "VIEW", COLOR_TEXT);
	i = 0;
	while (i < 5)
	{
		panel_draw_button(renderer, visu, &buttons[i]);
		i++;
	}
	hlineColor(renderer, 18, 374, 302, COLOR_PANEL_MUTED);
}

static void	panel_draw_playback(SDL_Renderer *renderer, t_visu *visu)
{
	t_panel_button	buttons[8];
	char			buffer[64];

	panel_build_buttons(buttons);
	stringColor(renderer, 18, 310, "PLAYBACK", COLOR_TEXT);
	panel_draw_button(renderer, visu, &buttons[5]);
	panel_draw_button(renderer, visu, &buttons[6]);
	snprintf(buffer, sizeof(buffer), "speed %s", anim_speed_label(visu));
	stringColor(renderer, 152, 324, buffer, COLOR_TEXT);
	stringColor(renderer, 18, 346, "TAB hide | SPACE play | arrows step",
		COLOR_TEXT);
}

static void	panel_draw_button(SDL_Renderer *renderer, t_visu *visu,
	t_panel_button *button)
{
	uint32_t	color;
	int			value;

	value = panel_action_value(visu, button->action_id);
	if (value == 1)
		color = GFX_COLOR(45, 90, 70, 255);
	else if (value == 0)
		color = GFX_COLOR(60, 60, 60, 255);
	else
		color = GFX_COLOR(55, 55, 55, 255);
	boxColor(renderer, button->rect.x, button->rect.y,
		button->rect.x + button->rect.w, button->rect.y + button->rect.h,
		color);
	rectangleColor(renderer, button->rect.x, button->rect.y,
		button->rect.x + button->rect.w, button->rect.y + button->rect.h,
		COLOR_PANEL_BORDER);
	panel_draw_button_label(renderer, button, value);
}

static void	panel_draw_button_label(SDL_Renderer *renderer,
	t_panel_button *button, int value)
{
	char	buffer[64];

	if (value == 1)
		snprintf(buffer, sizeof(buffer), "[ON ] %s", button->label);
	else if (value == 0)
		snprintf(buffer, sizeof(buffer), "[OFF] %s", button->label);
	else
		snprintf(buffer, sizeof(buffer), "%s", button->label);
	stringColor(renderer, button->rect.x + 6, button->rect.y + 5, buffer,
		COLOR_TEXT);
}

static int	panel_action_value(t_visu *visu, int action_id)
{
	if (action_id == PANEL_ACTION_LINKS)
		return (visu->settings.show_links);
	if (action_id == PANEL_ACTION_UNUSED_ELEMENTS)
		return (visu->settings.show_unused_elements);
	if (action_id == PANEL_ACTION_PATH_COLORS)
		return (visu->settings.color_paths);
	if (action_id == PANEL_ACTION_ROOM_NAMES)
		return (visu->settings.show_room_names);
	if (action_id == PANEL_ACTION_ANT_IDS)
		return (visu->settings.show_ant_ids);
	return (-1);
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

static int	panel_display_slide(t_visu *visu)
{
	if (visu->anim.transition.active
		&& visu->anim.transition.direction == 1
		&& visu->anim.current_slide < visu->anim.step_count)
		return (visu->anim.current_slide + 1);
	return (visu->anim.current_slide);
}

static void	panel_get_ant_counts(t_visu *visu, int *start, int *active,
	int *end)
{
	t_step	*step;
	t_move	*move;
	int		counts[3];
	size_t	i;

	counts[ANT_START] = visu->anim.start_count;
	counts[ANT_ACTIVE] = visu->anim.active_count;
	counts[ANT_END] = visu->anim.end_count;
	if (visu->anim.transition.active)
	{
		step = (t_step *)visu->anim.steps.data[visu->anim.transition.step_index];
		i = 0;
		while (i < step->moves.count)
		{
			move = (t_move *)step->moves.data[i];
			panel_apply_room_count(visu, move->from, counts, -1);
			panel_apply_room_count(visu, move->to, counts, 1);
			i++;
		}
	}
	*start = counts[ANT_START];
	*active = counts[ANT_ACTIVE];
	*end = counts[ANT_END];
}

static void	panel_apply_room_count(t_visu *visu, t_room *room, int *counts,
	int delta)
{
	if (room == visu->farm->start)
		counts[ANT_START] += delta;
	else if (room == visu->farm->end)
		counts[ANT_END] += delta;
	else
		counts[ANT_ACTIVE] += delta;
}

static int	panel_percent(int value, int total)
{
	return ((value * 100 + total / 2) / total);
}