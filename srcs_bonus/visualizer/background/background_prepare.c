#include "visualizer.h"

#define CLIP_LEFT 1
#define CLIP_RIGHT 2
#define CLIP_TOP 4
#define CLIP_BOTTOM 8

static int	line_clip_code(double x, double y, double min_x, double min_y,
				double max_x, double max_y);
static int	clip_line_to_rect(t_line_cmd *cmd, int min_x, int min_y,
				int max_x, int max_y);

void	background_prepare_links(t_visu *visu)
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
		if (visu->settings.show_unused_elements
			|| visu->background.link_used[i])
		{
			link = (t_link *)visu->farm->links.data[i];
			logical_to_pixel(link->a->x, link->a->y, &visu->camera,
				&cmd.x1, &cmd.y1);
			logical_to_pixel(link->b->x, link->b->y, &visu->camera,
				&cmd.x2, &cmd.y2);
			cmd.thickness = 2;
			if (visu->settings.color_paths)
				cmd.color = visu->background.link_color[i];
			else
				cmd.color = COLOR_LINK;
			if (background_line_visible(visu, &cmd))
				background_push_line(visu, &cmd);
		}
		i++;
	}
	end = SDL_GetPerformanceCounter();
	visu->profile.prepare_links_ms = profile_elapsed_ms(start, end);
}

void	background_prepare_rooms(t_visu *visu)
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
		if (room != visu->farm->start && room != visu->farm->end
			&& (visu->settings.show_unused_elements
				|| visu->background.room_used[room->id]))
		{
			logical_to_pixel(room->x, room->y, &visu->camera,
				&circle.x, &circle.y);
			circle.radius = radius;
			circle.fill_color = COLOR_ROOM;
			if (visu->settings.color_paths)
				circle.border_color = visu->background.room_color[room->id];
			else
				circle.border_color = COLOR_ROOM_BORDER;
			circle.has_border = 1;
			if (background_circle_visible(visu, &circle))
			{
				background_push_circle(visu, &circle);
				if (visu->settings.show_room_names)
				{
					text.x = circle.x + radius + 6;
					text.y = circle.y - 10;
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

int	background_line_visible(t_visu *visu, t_line_cmd *cmd)
{
	int	pad;
	int	min_x;
	int	min_y;
	int	max_x;
	int	max_y;

	pad = cmd->thickness + 4;
	min_x = -pad;
	min_y = -pad;
	max_x = visu->camera.win_width + pad;
	max_y = visu->camera.win_height + pad;
	return (clip_line_to_rect(cmd, min_x, min_y, max_x, max_y));
}

int	background_circle_visible(t_visu *visu, t_circle_cmd *cmd)
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

static int	clip_line_to_rect(t_line_cmd *cmd, int min_x, int min_y,
	int max_x, int max_y)
{
	double	x1;
	double	y1;
	double	x2;
	double	y2;
	double	x;
	double	y;
	int		code1;
	int		code2;
	int		out;

	x1 = cmd->x1;
	y1 = cmd->y1;
	x2 = cmd->x2;
	y2 = cmd->y2;
	while (1)
	{
		code1 = line_clip_code(x1, y1, min_x, min_y, max_x, max_y);
		code2 = line_clip_code(x2, y2, min_x, min_y, max_x, max_y);
		if (!(code1 | code2))
			break ;
		if (code1 & code2)
			return (0);
		if (code1)
			out = code1;
		else
			out = code2;
		if (out & CLIP_TOP)
		{
			x = x1 + (x2 - x1) * ((double)min_y - y1) / (y2 - y1);
			y = min_y;
		}
		else if (out & CLIP_BOTTOM)
		{
			x = x1 + (x2 - x1) * ((double)max_y - y1) / (y2 - y1);
			y = max_y;
		}
		else if (out & CLIP_RIGHT)
		{
			y = y1 + (y2 - y1) * ((double)max_x - x1) / (x2 - x1);
			x = max_x;
		}
		else
		{
			y = y1 + (y2 - y1) * ((double)min_x - x1) / (x2 - x1);
			x = min_x;
		}
		if (out == code1)
		{
			x1 = x;
			y1 = y;
		}
		else
		{
			x2 = x;
			y2 = y;
		}
	}
	cmd->x1 = (int)x1;
	cmd->y1 = (int)y1;
	cmd->x2 = (int)x2;
	cmd->y2 = (int)y2;
	return (1);
}

static int	line_clip_code(double x, double y, double min_x, double min_y,
	double max_x, double max_y)
{
	int	code;

	code = 0;
	if (x < min_x)
		code |= CLIP_LEFT;
	else if (x > max_x)
		code |= CLIP_RIGHT;
	if (y < min_y)
		code |= CLIP_TOP;
	else if (y > max_y)
		code |= CLIP_BOTTOM;
	return (code);
}