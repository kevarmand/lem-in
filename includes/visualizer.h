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

# define ANT_START 0
# define ANT_ACTIVE 1
# define ANT_END 2

# define GFX_COLOR(r, g, b, a) \
	(((uint32_t)(a) << 24) | ((uint32_t)(b) << 16) \
	| ((uint32_t)(g) << 8) | (uint32_t)(r))

# define COLOR_BACKGROUND GFX_COLOR(17, 17, 17, 255)
# define COLOR_LINK GFX_COLOR(119, 119, 119, 255)
# define COLOR_ROOM GFX_COLOR(75, 110, 140, 255)
# define COLOR_START GFX_COLOR(61, 220, 132, 255)
# define COLOR_END GFX_COLOR(255, 85, 85, 255)
# define COLOR_ROOM_BORDER GFX_COLOR(255, 255, 255, 255)
# define COLOR_TEXT GFX_COLOR(255, 255, 255, 255)
# define COLOR_PANEL_BG GFX_COLOR(25, 25, 25, 230)
# define COLOR_PANEL_BORDER GFX_COLOR(210, 210, 210, 255)

typedef struct s_visu_settings
{
	int			show_room_names;
	int			show_ant_ids;
	int			show_links;
	int			show_hud;
	int			show_controls;
}	t_visu_settings;

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
	int			current_slide;
	int			start_count;
	int			end_count;
	int			active_count;
	int			*active_ants;
	int			*active_pos;
	int			*ant_status;
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
	t_farm			*farm;
	t_anim			anim;
	t_camera		camera;
	t_layout		layout;
	t_vector		paths;
	t_visu_settings	settings;
	int				hover_ant;
	t_room			*hover_room;
}	t_visu;

int		parse_visu_input(t_farm *farm, t_visu *visu);
int		parse_move_line(int *err, char **line, t_farm *farm, t_visu *visu);

int		launch_visualizer(t_visu *visu);
void	visualizer_destroy(t_visu *visu);

void	camera_fit_farm(t_camera *camera, t_farm *farm);
void	logical_to_pixel(double x, double y, t_camera *camera, int *px, int *py);
void	pixel_to_logical(int px, int py, t_camera *camera, double *x, double *y);
int		logical_radius_to_pixel(double radius, t_camera *camera);
void	zoom_around_mouse(int mouse_x, int mouse_y, t_camera *camera,
			double factor);

void	timeline_reset(t_visu *visu);
void	timeline_next(t_visu *visu);
void	timeline_prev(t_visu *visu);

void	handle_events(t_visu *visu, int *running, int *need_redraw);
void	draw_scene(SDL_Renderer *renderer, t_visu *visu);
void	draw_ants(SDL_Renderer *renderer, t_visu *visu);

#endif