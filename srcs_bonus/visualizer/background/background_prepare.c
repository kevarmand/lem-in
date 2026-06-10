#include "visualizer.h"

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

int	background_line_visible(t_visu *visu, t_line_cmd *cmd)
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