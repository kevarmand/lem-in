/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flow_edge.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:23:45 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 16:23:46 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "solver.h"

static t_flow_edge	*new_edge(int from, int to, int cap, int cost)
{
	t_flow_edge	*edge;

	edge = malloc(sizeof(*edge));
	if (!edge)
		return (NULL);
	edge->from = from;
	edge->to = to;
	edge->cap = cap;
	edge->cost = cost;
	edge->real = 0;
	edge->room_from = -1;
	edge->room_to = -1;
	edge->mark = 0;
	edge->rev = NULL;
	return (edge);
}

static int	push_edge_pair(t_flow_graph *graph, t_flow_edge *edge,
	t_flow_edge *rev)
{
	if (vector_push_back(&graph->adj[edge->from], edge))
		return (ERR_MALLOC);
	if (vector_push_back(&graph->adj[rev->from], rev))
	{
		graph->adj[edge->from].count--;
		return (ERR_MALLOC);
	}
	return (ERR_NO_ERROR);
}

int	flow_graph_add_edge(t_flow_graph *graph, int from, int to, int cap,
	int cost)
{
	t_flow_edge	*edge;
	t_flow_edge	*rev;
	int			err;

	edge = new_edge(from, to, cap, cost);
	rev = new_edge(to, from, 0, -cost);
	if (!edge || !rev)
	{
		free(edge);
		free(rev);
		return (ERR_MALLOC);
	}
	edge->rev = rev;
	rev->rev = edge;
	err = push_edge_pair(graph, edge, rev);
	if (err)
	{
		free(edge);
		free(rev);
		return (err);
	}
	return (ERR_NO_ERROR);
}

int	flow_graph_add_real_edge(t_flow_graph *graph, t_room *from, t_room *to)
{
	t_flow_edge	*edge;
	t_flow_edge	*rev;
	int			err;

	edge = new_edge(graph->out_id[from->id], graph->in_id[to->id], 1, 1);
	rev = new_edge(graph->in_id[to->id], graph->out_id[from->id], 0, -1);
	if (!edge || !rev)
	{
		free(edge);
		free(rev);
		return (ERR_MALLOC);
	}
	edge->real = 1;
	edge->room_from = from->id;
	edge->room_to = to->id;
	edge->rev = rev;
	rev->rev = edge;
	err = push_edge_pair(graph, edge, rev);
	if (err)
	{
		free(edge);
		free(rev);
		return (err);
	}
	return (ERR_NO_ERROR);
}