/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:37:30 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 11:02:21 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"
static double	clamp_zoom(t_camera *camera, double zoom);
static void	get_farm_bounds(t_farm *farm, double *x_min, double *x_max,
				double *y_min, double *y_max);

void	camera_fit_farm(t_camera *camera, t_farm *farm) {
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
	double	width;
	double	height;
	double	usable_width;
	double	usable_height;
	double	margin_x;
	double	margin_y;
	double	zoom_x;
	double	zoom_y;

	get_farm_bounds(farm, &x_min, &x_max, &y_min, &y_max);
	width = x_max - x_min;
	height = y_max - y_min;
	if (width <= 0.0)
		width = 1.0;
	if (height <= 0.0)
		height = 1.0;
	margin_x = camera->win_width * VIEW_MARGIN_RATIO;
	margin_y = camera->win_height * VIEW_MARGIN_RATIO;
	usable_width = camera->win_width - (margin_x * 2.0);
	usable_height = camera->win_height - (margin_y * 2.0);
	zoom_x = usable_width / width;
	zoom_y = usable_height / height;
	if (zoom_x < zoom_y)
		camera->zoom = zoom_x;
	else
		camera->zoom = zoom_y;
	camera->min_zoom = camera->zoom * ZOOM_MIN_FACTOR;
	camera->max_zoom = ZOOM_MAX_PIXELS_PER_UNIT;
	if (camera->max_zoom < camera->zoom)
		camera->max_zoom = camera->zoom;
	camera->x_offset = x_min - (margin_x / camera->zoom);
	camera->y_offset = y_max + (margin_y / camera->zoom);
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
	return ((int)(radius * camera->zoom));
}

void	zoom_around_mouse(int mouse_x, int mouse_y, t_camera *camera,
	double factor)
{
	double	before_x;
	double	before_y;
	double	after_x;
	double	after_y;

	pixel_to_logical(mouse_x, mouse_y, camera, &before_x, &before_y);
	camera->zoom = clamp_zoom(camera, camera->zoom * factor);
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

static double	clamp_zoom(t_camera *camera, double zoom)
{
	if (zoom < camera->min_zoom)
		return (camera->min_zoom);
	if (zoom > camera->max_zoom)
		return (camera->max_zoom);
	return (zoom);
}