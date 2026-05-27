/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flow_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:23:29 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 16:23:30 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "solver.h"
#include "libft.h"

static int	flow_graph_alloc(t_flow_graph *graph, t_farm *farm)
{
	int	i;

	graph->farm = farm;
	graph->room_count = (int)farm->rooms.count;
	graph->node_count = graph->room_count * 2;
	graph->adj = ft_calloc(graph->node_count, sizeof(*graph->adj));
	graph->in_id = ft_calloc(graph->room_count, sizeof(*graph->in_id));
	graph->out_id = ft_calloc(graph->room_count, sizeof(*graph->out_id));
	if (!graph->adj || !graph->in_id || !graph->out_id)
		return (ERR_MALLOC);
	i = 0;
	while (i < graph->node_count)
	{
		if (vector_init(&graph->adj[i], 4))
			return (ERR_MALLOC);
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	flow_graph_set_rooms(t_flow_graph *graph)
{
	t_room	*room;
	int		i;
	int		cap;

	i = 0;
	while (i < graph->room_count)
	{
		room = graph->farm->rooms.data[i];
		graph->in_id[room->id] = room->id * 2;
		graph->out_id[room->id] = room->id * 2 + 1;
		i++;
	}
	graph->source = graph->out_id[graph->farm->start->id];
	graph->sink = graph->in_id[graph->farm->end->id];
	i = 0;
	while (i < graph->room_count)
	{
		room = graph->farm->rooms.data[i];
		cap = 1;
		if (room == graph->farm->start || room == graph->farm->end)
			cap = FLOW_INF;
		if (flow_graph_add_edge(graph, graph->in_id[room->id],
				graph->out_id[room->id], cap, 0))
			return (ERR_MALLOC);
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	flow_graph_set_links(t_flow_graph *graph)
{
	t_link	*link;
	size_t	i;

	i = 0;
	while (i < graph->farm->links.count)
	{
		link = graph->farm->links.data[i];
		if (flow_graph_add_real_edge(graph, link->a, link->b)
			|| flow_graph_add_real_edge(graph, link->b, link->a))
			return (ERR_MALLOC);
		i++;
	}
	return (ERR_NO_ERROR);
}

int	flow_graph_init(t_flow_graph *graph, t_farm *farm)
{
	int	err;

	graph->farm = NULL;
	graph->adj = NULL;
	graph->in_id = NULL;
	graph->out_id = NULL;
	graph->room_count = 0;
	graph->node_count = 0;
	graph->source = 0;
	graph->sink = 0;
	err = flow_graph_alloc(graph, farm);
	if (!err)
		err = flow_graph_set_rooms(graph);
	if (!err)
		err = flow_graph_set_links(graph);
	if (err)
		flow_graph_destroy(graph);
	return (err);
}

void	flow_graph_destroy(t_flow_graph *graph)
{
	t_flow_edge	*edge;
	int			i;
	size_t		j;

	if (graph->adj)
	{
		i = 0;
		while (i < graph->node_count)
		{
			j = 0;
			while (j < graph->adj[i].count)
			{
				edge = graph->adj[i].data[j];
				free(edge);
				j++;
			}
			vector_destroy(&graph->adj[i]);
			i++;
		}
	}
	free(graph->adj);
	free(graph->in_id);
	free(graph->out_id);
	graph->adj = NULL;
	graph->in_id = NULL;
	graph->out_id = NULL;
}