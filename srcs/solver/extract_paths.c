/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_paths.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:24:43 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 11:40:37 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "solver.h"
#include "libft.h"

#define PATH_INITIAL_CAPACITY 16

static void	clear_marks(t_flow_graph *graph) {
	t_flow_edge	*edge;
	int			i;
	size_t		j;

	i = 0;
	while (i < graph->node_count)
	{
		j = 0;
		while (j < graph->adj[i].count)
		{
			edge = graph->adj[i].data[j];
			edge->mark = 0;
			j++;
		}
		i++;
	}
}

static t_flow_edge	*find_used_edge(t_flow_graph *graph, int room_id) {
	t_flow_edge	*edge;
	int			node;
	size_t		i;

	node = graph->out_id[room_id];
	i = 0;
	while (i < graph->adj[node].count)
	{
		edge = graph->adj[node].data[i];
		if (edge->real && edge->rev->cap > 0 && !edge->mark)
			return (edge);
		i++;
	}
	return (NULL);
}

static int	extract_one_path(t_flow_graph *graph, t_path **out,
	t_flow_edge *first) {
	t_path		*path;
	t_room		*room;
	t_flow_edge	*edge;
	int			current;

	path = path_create(PATH_INITIAL_CAPACITY);
	if (!path)
		return (ERR_MALLOC);
	current = graph->farm->start->id;
	room = graph->farm->rooms.data[current];
	if (path_push_room(path, room))
		return (path_destroy(path), ERR_MALLOC);
	edge = first;
	while (current != graph->farm->end->id)
	{
		if (!edge)
			return (path_destroy(path), ERR_LINK);
		edge->mark = 1;
		current = edge->room_to;
		room = graph->farm->rooms.data[current];
		if (path_push_room(path, room))
			return (path_destroy(path), ERR_MALLOC);
		path->len++;
		edge = find_used_edge(graph, current);
	}
	*out = path;
	return (ERR_NO_ERROR);
}

static int	extract_from_start(t_flow_graph *graph, t_solution *solution) {
	t_flow_edge	*edge;
	t_path		*path;
	int			node;
	size_t		i;
	int			err;

	node = graph->out_id[graph->farm->start->id];
	i = 0;
	while (i < graph->adj[node].count)
	{
		edge = graph->adj[node].data[i];
		if (edge->real && edge->rev->cap > 0 && !edge->mark)
		{
			path = NULL;
			err = extract_one_path(graph, &path, edge);
			if (!err)
				err = solution_add_path(solution, path);
			if (err)
			{
				if (path)
					path_destroy(path);
				return (err);
			}
		}
		i++;
	}
	return (ERR_NO_ERROR);
}

int	extract_solution(t_flow_graph *graph, t_solution *solution) {
	int	err;

	clear_marks(graph);
	err = solution_init(solution, 4);
	if (err)
		return (err);
	err = extract_from_start(graph, solution);
	if (err)
	{
		solution_destroy(solution);
		return (err);
	}
	return (ERR_NO_ERROR);
}