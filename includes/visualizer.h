/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vissualizer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:14:50 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:38:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VISUALIZER_H
# define VISUALIZER_H

# include <stdint.h>
# include <SDL2/SDL.h>
# include "farm.h"
# include "parsing.h"
# include "SDL2_gfxPrimitives.h"

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600

# define LAYOUT_RAW 0
# define LAYOUT_PATHS 1

typedef struct s_ant_state
{
	int			id;
	t_room		*room;
	int			path_id;
	uint32_t	color;
	int			arrived;
}	t_ant_state;

typedef struct s_move
{
	int			ant_id;
	t_room		*from;
	t_room		*to;
	int			path_id;
}	t_move;

typedef struct s_step
{
	t_vector	moves;
}	t_step;

typedef struct s_anim
{
	t_ant_state	*ants;
	t_vector	steps;
	int			step_count;
	int			current_step;
	double		time;
	double		step_duration;
	int			paused;
}	t_anim;

typedef struct s_visu_path
{
	int			id;
	t_vector	rooms;
	uint32_t	color;
}	t_visu_path;

typedef struct s_camera
{
	double		zoom;
	double		x_offset;
	double		y_offset;
	int			win_width;
	int			win_height;
}	t_camera;

typedef struct s_layout_node
{
	t_room		*room;
	double		x;
	double		y;
}	t_layout_node;

typedef struct s_layout
{
	t_vector	nodes;
	int			mode;
}	t_layout;

typedef struct s_visu
{
	t_farm		*farm;
	t_anim		anim;
	t_camera	camera;
	t_layout	layout;
	t_vector	paths;
	int			show_names;
	int			show_ant_ids;
	int			show_links;
	int			hover_ant;
	t_room		*hover_room;
}	t_visu;

int		parse_visu_input(t_farm *farm, t_visu *visu);
int		parse_move_line(int *err, char **line, t_farm *farm, t_visu *visu);
int		launch_visualizer(t_visu *visu);
void	visualizer_destroy(t_visu *visu);

#endif