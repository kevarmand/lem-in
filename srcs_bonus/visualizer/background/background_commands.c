#include <stdlib.h>
#include "visualizer.h"
#include "error.h"

int	background_init_commands(t_visu *visu)
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

void	background_prepare_commands(t_visu *visu)
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

void	background_push_line(t_visu *visu, t_line_cmd *cmd)
{
	int	i;

	i = visu->background.line_count;
	visu->background.lines[i] = *cmd;
	visu->background.line_count++;
}

void	background_push_circle(t_visu *visu, t_circle_cmd *cmd)
{
	int	i;

	i = visu->background.circle_count;
	visu->background.circles[i] = *cmd;
	visu->background.circle_count++;
}

void	background_push_text(t_visu *visu, t_text_cmd *cmd)
{
	int	i;

	i = visu->background.text_count;
	visu->background.texts[i] = *cmd;
	visu->background.text_count++;
}