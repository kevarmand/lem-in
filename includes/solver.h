/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:14:22 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 11:38:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_H
# define SOLVER_H

# include "farm.h"

# define FLOW_INF 1073741823
# define FLOW_DIST_INF 1073741823

typedef struct s_path
{
	t_vector	rooms;
	int			len;
	int			ants;
}	t_path;

typedef struct s_solution
{
	t_vector	paths;
	int			path_count;
	int			rounds;
	int			total_len;
}	t_solution;

typedef struct s_flow_edge
{
	int					from;
	int					to;
	int					cap;
	int					cost;
	int					real;
	int					room_from;
	int					room_to;
	int					mark;
	struct s_flow_edge	*rev;
}	t_flow_edge;

typedef struct s_flow_graph
{
	t_farm		*farm;
	t_vector	*adj;
	int			*in_id;
	int			*out_id;
	int			room_count;
	int			node_count;
	int			source;
	int			sink;
}	t_flow_graph;

typedef struct s_spfa
{
	int			*dist;
	int			*in_queue;
	int			*parent_node;
	int			*queue;
	t_flow_edge	**parent_edge;
}	t_spfa;

typedef struct s_solver
{
	t_farm			*farm;
	t_flow_graph	flow;
	t_vector		bests;
}	t_solver;

int			solve_farm(t_farm *farm, t_solution *solution);
void		solution_destroy(t_solution *solution);
int			print_solution(t_farm *farm, t_solution *solution);

int			solution_init(t_solution *solution, int capacity);
int			solution_clone(t_solution *dst, t_solution *src);
int			solution_add_path(t_solution *solution, t_path *path);
t_path		*path_create(int capacity);
int			path_push_room(t_path *path, t_room *room);
void		path_destroy(void *ptr);

int			flow_graph_init(t_flow_graph *graph, t_farm *farm);
void		flow_graph_destroy(t_flow_graph *graph);
int			flow_graph_add_edge(t_flow_graph *graph, int from, int to,
				int cap, int cost);
int			flow_graph_add_real_edge(t_flow_graph *graph, t_room *from,
				t_room *to);

int			min_cost_augment(t_flow_graph *graph, t_spfa *spfa, int *found);
int			spfa_init(t_spfa *spfa, int node_count);
void		spfa_destroy(t_spfa *spfa);

int			extract_solution(t_flow_graph *graph, t_solution *solution);
int			choose_solution(t_farm *farm, t_vector *bests,
				t_solution *solution);
int			solution_rounds(t_solution *solution, int ants);
void		assign_ants(t_solution *solution, int ants);

#endif