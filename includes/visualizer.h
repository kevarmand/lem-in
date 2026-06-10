/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 13:42:36 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 13:49:08 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VISUALIZER_H
# define VISUALIZER_H

# include <stdint.h>
# include <SDL2/SDL.h>
# include "farm.h"
# include "parsing.h"
# include "SDL2_gfxPrimitives.h"

/*
** ============================================================================
** Window / layout constants
** ============================================================================
*/

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080

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
# define COLOR_PANEL_BG GFX_COLOR(25, 25, 25, 235)
# define COLOR_PANEL_BORDER GFX_COLOR(210, 210, 210, 255)
# define COLOR_PANEL_MUTED GFX_COLOR(80, 80, 80, 255)
# define COLOR_PANEL_START GFX_COLOR(45, 72, 92, 255)
# define COLOR_PANEL_ACTIVE GFX_COLOR(92, 72, 43, 255)
# define COLOR_PANEL_END GFX_COLOR(42, 86, 67, 255)
# define COLOR_TERMINAL_EMPTY GFX_COLOR(42, 46, 54, 255)
# define COLOR_TERMINAL_FULL GFX_COLOR(35, 205, 220, 255)

# define ZOOM_MIN_FACTOR 0.40
# define ZOOM_MAX_PIXELS_PER_UNIT 250.0
# define VIEW_MARGIN_RATIO 0.08

# define ROOM_RADIUS_LOGICAL 0.16
# define ROOM_RADIUS_MIN 8
# define ROOM_RADIUS_MAX 26

# define ANT_RADIUS_RATIO 0.45
# define TERMINAL_WIDTH_RATIO 5
# define TERMINAL_HEIGHT_RATIO 3

# define ANIM_STEP_DURATION_MS 300
# define ANIM_SPEED_COUNT 7
# define ANIM_SPEED_DEFAULT_INDEX 2

# define PANEL_ACTION_LINKS 1
# define PANEL_ACTION_UNUSED_ELEMENTS 2
# define PANEL_ACTION_PATH_COLORS 3
# define PANEL_ACTION_ROOM_NAMES 4
# define PANEL_ACTION_ANT_IDS 5
# define PANEL_ACTION_SPEED_DOWN 6
# define PANEL_ACTION_SPEED_UP 7

/*
** ============================================================================
** Core visualizer state
** ============================================================================
*/

typedef struct s_visu_settings
{
	int			show_room_names;
	int			show_ant_ids;
	int			show_links;
	int			show_unused_elements;
	int			color_paths;
	int			show_overlay;
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
	t_link		*link;
	int			path_id;
}	t_move;

typedef struct s_step
{
	t_vector	moves;
}	t_step;

/*
** ============================================================================
** Animation / timeline state
** ============================================================================
*/

typedef struct s_transition_ant
{
	int			ant_id;
	t_room		*from;
	t_room		*to;
	uint32_t	color;
}	t_transition_ant;

typedef struct s_transition
{
	int					active;
	int					direction;
	int					step_index;
	Uint32				start_ms;
	Uint32				duration_ms;
	double				progress;
	t_transition_ant	*ants;
	int					count;
	int					*pos;
}	t_transition;

typedef struct s_anim
{
	t_ant_state		*ants;
	t_vector		steps;
	int				step_count;
	int				current_slide;
	int				start_count;
	int				end_count;
	int				active_count;
	int				*active_ants;
	int				*active_pos;
	int				*ant_status;
	int				playing;
	t_transition	transition;
	int				speed_index;
	Uint32			step_duration_ms;
}	t_anim;

/*
** ============================================================================
** Camera / drawing commands
** ============================================================================
*/

typedef struct s_camera
{
	double		zoom;
	double		min_zoom;
	double		max_zoom;
	double		x_offset;
	double		y_offset;
	int			win_width;
	int			win_height;
}	t_camera;

typedef struct s_line_cmd
{
	int			x1;
	int			y1;
	int			x2;
	int			y2;
	int			thickness;
	uint32_t	color;
}	t_line_cmd;

typedef struct s_circle_cmd
{
	int			x;
	int			y;
	int			radius;
	uint32_t	fill_color;
	uint32_t	border_color;
	int			has_border;
}	t_circle_cmd;

typedef struct s_text_cmd
{
	int			x;
	int			y;
	char		*text;
	uint32_t	color;
}	t_text_cmd;

/*
** ============================================================================
** Background cache
** ============================================================================
*/

typedef struct s_link_id_slot
{
	t_link		*link;
	int			id;
}	t_link_id_slot;

typedef struct s_background
{
	SDL_Texture		*texture;
	int				dirty;
	int				width;
	int				height;
	t_line_cmd		*lines;
	int				line_count;
	int				line_capacity;
	t_circle_cmd	*circles;
	int				circle_count;
	int				circle_capacity;
	t_text_cmd		*texts;
	int				text_count;
	int				text_capacity;
	SDL_Texture		*circle_fill[ROOM_RADIUS_MAX + 1];
	SDL_Texture		*circle_border[ROOM_RADIUS_MAX + 1];
	int				*room_used;
	uint32_t		*room_color;
	int				*link_used;
	uint32_t		*link_color;
	t_link_id_slot	*link_ids;
	int				link_id_capacity;
}	t_background;

/*
** ============================================================================
** Profiling / panel
** ============================================================================
*/

typedef struct s_profile
{
	int		enabled;
	double	background_ms;
	double	static_map_ms;
	double	links_ms;
	double	rooms_ms;
	double	prepare_ms;
	double	prepare_links_ms;
	double	prepare_rooms_ms;
	double	prepare_texts_ms;
	double	draw_links_ms;
	double	draw_rooms_ms;
	double	draw_texts_ms;
	double	terminals_ms;
	double	ants_ms;
	double	hud_ms;
	double	frame_ms;
}	t_profile;

typedef struct s_panel_button
{
	SDL_Rect	rect;
	int			action_id;
	const char	*label;
}	t_panel_button;

/*
** ============================================================================
** Main visualizer object
** ============================================================================
*/

typedef struct s_visu
{
	t_farm			*farm;
	t_anim			anim;
	t_camera		camera;
	t_background	background;
	t_profile		profile;
	t_visu_settings	settings;
	int				*path_color_index;
	int				path_count;
}	t_visu;

/*
** ============================================================================
** Parsing
** ============================================================================
*/

/**
 * @brief Parse the mandatory lem-in output and initialize visualizer state.
 * @param farm Farm object filled while parsing rooms and links.
 * @param visu Visualizer object receiving steps, ants and settings.
 * @return ERR_NO_ERROR on success, otherwise an error code.
 * @note This parser expects the output format produced by lem-in.
 */
int			parse_visu_input(t_farm *farm, t_visu *visu);

/**
 * @brief Parse one movement line such as "L1-room L2-room".
 * @param err Shared parser error.
 * @param line Current owned input line.
 * @param farm Parsed farm.
 * @param visu Visualizer state.
 * @return 1 if the line was consumed, 0 otherwise.
 */
int			parse_move_line(int *err, char **line, t_farm *farm, t_visu *visu);

/*
** ============================================================================
** Visualizer lifecycle
** ============================================================================
*/

/**
 * @brief Launch SDL and run the visualizer main loop.
 * @param visu Fully parsed visualizer state.
 * @return ERR_NO_ERROR on clean exit, otherwise an error code.
 * @note SDL textures are destroyed before the renderer/window are destroyed.
 */
int			launch_visualizer(t_visu *visu);

/**
 * @brief Destroy all non-SDL-parser visualizer allocations.
 * @param visu Visualizer state to clean.
 * @note Safe to call even if SDL background resources were already destroyed.
 */
void		visualizer_destroy(t_visu *visu);

/*
** ============================================================================
** Reorganization / layout
** ============================================================================
*/

/**
 * @brief Reassign room coordinates for a more readable path-based view.
 * @param visu Visualizer state containing the parsed farm and moves.
 * @return ERR_NO_ERROR on success, otherwise an error code.
 * @note This mutates room coordinates inside the visualizer copy of the farm.
 */
int			visu_reorganize_rooms(t_visu *visu);

int			visu_reorg_build_order(t_farm *farm, int *dist, int *used,
				int *order);

/*
** ============================================================================
** Camera helpers
** ============================================================================
*/

void		camera_fit_farm(t_camera *camera, t_farm *farm);
void		logical_to_pixel(double x, double y, t_camera *camera, int *px,
				int *py);
void		pixel_to_logical(int px, int py, t_camera *camera, double *x,
				double *y);
int			logical_radius_to_pixel(double radius, t_camera *camera);
void		zoom_around_mouse(int mouse_x, int mouse_y, t_camera *camera,
				double factor);

/*
** ============================================================================
** Visual metrics / colors
** ============================================================================
*/

int			visu_room_radius(t_visu *visu);
int			visu_ant_radius(t_visu *visu);
int			visu_terminal_width(t_visu *visu);
int			visu_terminal_height(t_visu *visu);

uint32_t	visu_mix_color(uint32_t empty, uint32_t full, int value, int max);
uint32_t	visu_terminal_color(t_visu *visu, int count);
uint32_t	visu_ant_path_color(t_visu *visu, int path_id, int ant_id);

/*
** ============================================================================
** Timeline / animation
** ============================================================================
*/

void		timeline_reset(t_visu *visu);
void		timeline_next(t_visu *visu);
void		timeline_prev(t_visu *visu);

void		anim_toggle_play(t_visu *visu);
void		anim_update(t_visu *visu);
void		anim_speed_up(t_visu *visu);
void		anim_speed_down(t_visu *visu);
const char	*anim_speed_label(t_visu *visu);

/*
** ============================================================================
** Events / draw
** ============================================================================
*/

void		handle_events(t_visu *visu, int *running, int *need_redraw);
int			draw_scene(SDL_Renderer *renderer, t_visu *visu);
void		draw_dynamic_terminals(SDL_Renderer *renderer, t_visu *visu);
void		draw_ants(SDL_Renderer *renderer, t_visu *visu);

void		panel_draw(SDL_Renderer *renderer, t_visu *visu);
int			panel_handle_event(SDL_Event *event, t_visu *visu,
				int *need_redraw);

/*
** ============================================================================
** Background lifecycle
** ============================================================================
*/

int			background_init(SDL_Renderer *renderer, t_visu *visu);
void		background_invalidate(t_visu *visu);
void		background_destroy(t_visu *visu);
int			background_rebuild(SDL_Renderer *renderer, t_visu *visu);
void		background_render(SDL_Renderer *renderer, t_visu *visu);

/*
** ============================================================================
** Background commands / marks
** ============================================================================
*/

int			background_init_commands(t_visu *visu);
void		background_prepare_commands(t_visu *visu);
void		background_push_line(t_visu *visu, t_line_cmd *cmd);
void		background_push_circle(t_visu *visu, t_circle_cmd *cmd);
void		background_push_text(t_visu *visu, t_text_cmd *cmd);

int			background_init_marks(t_visu *visu);
int			background_init_link_ids(t_visu *visu);
int			background_mark_solution(t_visu *visu);
int			background_link_id(t_visu *visu, t_link *link);

void		background_prepare_links(t_visu *visu);
void		background_prepare_rooms(t_visu *visu);
int			background_line_visible(t_visu *visu, t_line_cmd *cmd);
int			background_circle_visible(t_visu *visu, t_circle_cmd *cmd);

/*
** ============================================================================
** Background drawing / sprites
** ============================================================================
*/

void		background_draw_commands(SDL_Renderer *renderer, t_visu *visu);
void		background_draw_circle_sprite(SDL_Renderer *renderer,
				t_visu *visu, t_circle_cmd *cmd);
void		background_set_texture_color(SDL_Texture *texture, uint32_t color);

int			background_init_circle_cache(SDL_Renderer *renderer, t_visu *visu);
void		background_destroy_circle_cache(t_visu *visu);

/*
** ============================================================================
** Profiling helpers
** ============================================================================
*/

double		profile_elapsed_ms(Uint64 start, Uint64 end);

#endif