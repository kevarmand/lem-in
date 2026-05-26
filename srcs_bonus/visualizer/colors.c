/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 11:20:43 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 11:46:40 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "visualizer.h"

#define GOLDEN_RATIO 0.618033988749895

static int		color_r(uint32_t color);
static int		color_g(uint32_t color);
static int		color_b(uint32_t color);
static int		color_a(uint32_t color);
static int		clamp_color(int value);
static int		mix_component(int a, int b, int value, int max);
static uint32_t	path_base_color(int index);
static uint32_t	path_palette_color(int index);
static uint32_t	path_generated_color(int index);
static uint32_t	color_variation(uint32_t color, int ant_id);
static uint32_t	hsv_to_color(double h, double s, double v);
static int		path_index(t_visu *visu, int path_id);

uint32_t	visu_mix_color(uint32_t empty, uint32_t full, int value, int max)
{
	int	r;
	int	g;
	int	b;
	int	a;

	if (value < 0)
		value = 0;
	if (value > max)
		value = max;
	if (max <= 0)
		return (empty);
	r = mix_component(color_r(empty), color_r(full), value, max);
	g = mix_component(color_g(empty), color_g(full), value, max);
	b = mix_component(color_b(empty), color_b(full), value, max);
	a = mix_component(color_a(empty), color_a(full), value, max);
	return (GFX_COLOR(r, g, b, a));
}

uint32_t	visu_terminal_color(t_visu *visu, int count)
{
	return (visu_mix_color(COLOR_TERMINAL_EMPTY, COLOR_TERMINAL_FULL,
			count, visu->farm->ants));
}

uint32_t	visu_ant_path_color(t_visu *visu, int path_id, int ant_id)
{
	return (color_variation(path_base_color(path_index(visu, path_id)),
			ant_id));
}

static int	path_index(t_visu *visu, int path_id)
{
	if (visu->path_color_index[path_id] == -1)
	{
		visu->path_color_index[path_id] = visu->path_count;
		visu->path_count++;
	}
	return (visu->path_color_index[path_id]);
}

static uint32_t	path_base_color(int index)
{
	if (index < 8)
		return (path_palette_color(index));
	return (path_generated_color(index));
}

static uint32_t	path_palette_color(int index)
{
	static uint32_t	palette[8] = {
		GFX_COLOR(35, 205, 220, 255),
		GFX_COLOR(245, 151, 67, 255),
		GFX_COLOR(166, 116, 255, 255),
		GFX_COLOR(94, 214, 120, 255),
		GFX_COLOR(236, 94, 170, 255),
		GFX_COLOR(240, 212, 90, 255),
		GFX_COLOR(94, 170, 255, 255),
		GFX_COLOR(79, 220, 180, 255)
	};

	return (palette[index]);
}

static uint32_t	path_generated_color(int index)
{
	double	h;

	h = fmod((double)index * GOLDEN_RATIO, 1.0);
	return (hsv_to_color(h, 0.72, 0.92));
}

static uint32_t	color_variation(uint32_t color, int ant_id)
{
	double	wave;
	double	factor;
	int		r;
	int		g;
	int		b;

	wave = sin((double)ant_id * 1.61803398875);
	factor = 1.0 + wave * 0.08;
	r = clamp_color((int)(color_r(color) * factor));
	g = clamp_color((int)(color_g(color) * factor));
	b = clamp_color((int)(color_b(color) * factor));
	return (GFX_COLOR(r, g, b, color_a(color)));
}

static uint32_t	hsv_to_color(double h, double s, double v)
{
	double	c;
	double	x;
	double	m;
	double	r;
	double	g;
	double	b;
	int		sector;

	c = v * s;
	sector = (int)(h * 6.0);
	x = c * (1.0 - fabs(fmod(h * 6.0, 2.0) - 1.0));
	m = v - c;
	r = 0.0;
	g = 0.0;
	b = 0.0;
	if (sector == 0)
	{
		r = c;
		g = x;
	}
	else if (sector == 1)
	{
		r = x;
		g = c;
	}
	else if (sector == 2)
	{
		g = c;
		b = x;
	}
	else if (sector == 3)
	{
		g = x;
		b = c;
	}
	else if (sector == 4)
	{
		r = x;
		b = c;
	}
	else
	{
		r = c;
		b = x;
	}
	return (GFX_COLOR((int)((r + m) * 255.0), (int)((g + m) * 255.0),
			(int)((b + m) * 255.0), 255));
}

static int	color_r(uint32_t color)
{
	return (color & 0xFF);
}

static int	color_g(uint32_t color)
{
	return ((color >> 8) & 0xFF);
}

static int	color_b(uint32_t color)
{
	return ((color >> 16) & 0xFF);
}

static int	color_a(uint32_t color)
{
	return ((color >> 24) & 0xFF);
}

static int	mix_component(int a, int b, int value, int max)
{
	return (a + ((b - a) * value) / max);
}

static int	clamp_color(int value)
{
	if (value < 0)
		return (0);
	if (value > 255)
		return (255);
	return (value);
}