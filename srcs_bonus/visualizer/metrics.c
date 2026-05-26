/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metrics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 10:46:50 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 10:46:51 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"

static int	clamp_int(int value, int min, int max);

int	visu_room_radius(t_visu *visu)
{
	int	radius;

	radius = logical_radius_to_pixel(ROOM_RADIUS_LOGICAL, &visu->camera);
	return (clamp_int(radius, ROOM_RADIUS_MIN, ROOM_RADIUS_MAX));
}

int	visu_ant_radius(t_visu *visu)
{
	int	radius;

	radius = (int)(visu_room_radius(visu) * ANT_RADIUS_RATIO);
	if (radius < 1)
		return (1);
	return (radius);
}

int	visu_terminal_width(t_visu *visu)
{
	return (visu_room_radius(visu) * TERMINAL_WIDTH_RATIO);
}

int	visu_terminal_height(t_visu *visu)
{
	return (visu_room_radius(visu) * TERMINAL_HEIGHT_RATIO);
}

static int	clamp_int(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}