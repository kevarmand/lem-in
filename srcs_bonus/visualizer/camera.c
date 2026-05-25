/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:37:30 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 14:37:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"

static void	get_farm_bounds(t_farm *farm, double *x_min, double *x_max,
				double *y_min, double *y_max);
static void	apply_margin(double *x_min, double *x_max,
				double *y_min, double *y_max);

void	camera_fit_farm(t_camera *camera, t_farm *farm)
{
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
	double	width;
	double	height;
	double	zoom_x;
	double	zoom_y;

	get_farm_bounds(farm, &x_min, &x_max, &y_min, &y_max);
	apply_margin(&x_min, &x_max, &y_min, &y_max);
	width = x_max - x_min;
	height = y_max - y_min;
	if (width <= 0.0)
		width = 1.0;
	if (height <= 0.0)
		height = 1.0;
	zoom_x = camera->win_width / width;
	zoom_y = camera->win_height / height;
	if (zoom_x < zoom_y)
		camera->zoom = zoom_x;
	else
		camera->zoom = zoom_y;
	camera->x_offset = x_min;
	camera->y_offset = y_max;
}

void	logical_to_pixel(double x, double y, t_camera *camera, int *px, int *py)
{
	*px = (int)((x - camera->x_offset) * camera->zoom);
	*py = (int)((-y + camera->y_offset) * camera->zoom);
}

void	pixel_to_logical(int px, int py, t_camera *camera, double *x, double *y)
{
	*x = ((double)px / camera->zoom) + camera->x_offset;
	*y = -((double)py / camera->zoom) + camera->y_offset;
}

int	logical_radius_to_pixel(double radius, t_camera *camera)
{
	int	out;

	out = (int)(radius * camera->zoom);
	if (out < 6)
		return (6);
	if (out > 25)
		return (25);
	return (out);
}

void	zoom_around_mouse(int mouse_x, int mouse_y, t_camera *camera,
	double factor)
{
	double	before_x;
	double	before_y;
	double	after_x;
	double	after_y;

	pixel_to_logical(mouse_x, mouse_y, camera, &before_x, &before_y);
	camera->zoom *= factor;
	if (camera->zoom < 1.0)
		camera->zoom = 1.0;
	pixel_to_logical(mouse_x, mouse_y, camera, &after_x, &after_y);
	camera->x_offset += before_x - after_x;
	camera->y_offset += before_y - after_y;
}

static void	get_farm_bounds(t_farm *farm, double *x_min, double *x_max,
	double *y_min, double *y_max)
{
	t_room	*room;
	size_t	i;

	room = (t_room *)farm->rooms.data[0];
	*x_min = room->x;
	*x_max = room->x;
	*y_min = room->y;
	*y_max = room->y;
	i = 1;
	while (i < farm->rooms.count)
	{
		room = (t_room *)farm->rooms.data[i];
		if (room->x < *x_min)
			*x_min = room->x;
		if (room->x > *x_max)
			*x_max = room->x;
		if (room->y < *y_min)
			*y_min = room->y;
		if (room->y > *y_max)
			*y_max = room->y;
		i++;
	}
}

static void	apply_margin(double *x_min, double *x_max,
	double *y_min, double *y_max)
{
	*x_min -= 1.0;
	*x_max += 1.0;
	*y_min -= 1.0;
	*y_max += 1.0;
}