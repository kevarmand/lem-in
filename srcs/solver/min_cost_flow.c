/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_cost_flow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:24:30 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 11:39:21 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "solver.h"
#include "libft.h"

int	spfa_init(t_spfa *spfa, int node_count) {
	spfa->dist = NULL;
	spfa->in_queue = NULL;
	spfa->parent_node = NULL;
	spfa->parent_edge = NULL;
	spfa->queue = NULL;
	spfa->dist = ft_calloc(node_count, sizeof(*spfa->dist));
	spfa->in_queue = ft_calloc(node_count, sizeof(*spfa->in_queue));
	spfa->parent_node = ft_calloc(node_count, sizeof(*spfa->parent_node));
	spfa->parent_edge = ft_calloc(node_count, sizeof(*spfa->parent_edge));
	spfa->queue = ft_calloc(node_count, sizeof(*spfa->queue));
	if (!spfa->dist || !spfa->in_queue || !spfa->parent_node
		|| !spfa->parent_edge || !spfa->queue)
	{
		spfa_destroy(spfa);
		return (ERR_MALLOC);
	}
	return (ERR_NO_ERROR);
}

void	spfa_destroy(t_spfa *spfa) {
	free(spfa->dist);
	free(spfa->in_queue);
	free(spfa->parent_node);
	free(spfa->parent_edge);
	free(spfa->queue);
	spfa->dist = NULL;
	spfa->in_queue = NULL;
	spfa->parent_node = NULL;
	spfa->parent_edge = NULL;
	spfa->queue = NULL;
}

static void	spfa_reset(t_spfa *spfa, int node_count) {
	int	i;

	i = 0;
	while (i < node_count)
	{
		spfa->dist[i] = FLOW_DIST_INF;
		spfa->in_queue[i] = 0;
		spfa->parent_node[i] = -1;
		spfa->parent_edge[i] = NULL;
		i++;
	}
}

static void	spfa_push(t_spfa *spfa, int *tail, int node, int node_count) {
	spfa->queue[*tail] = node;
	*tail = (*tail + 1) % node_count;
	spfa->in_queue[node] = 1;
}

static int	spfa_pop(t_spfa *spfa, int *head, int node_count) {
	int	node;

	node = spfa->queue[*head];
	*head = (*head + 1) % node_count;
	spfa->in_queue[node] = 0;
	return (node);
}

static void	spfa_relax_edges(t_flow_graph *graph, t_spfa *spfa, int node,
	int *tail) {
	t_flow_edge	*edge;
	size_t		i;

	i = 0;
	while (i < graph->adj[node].count)
	{
		edge = graph->adj[node].data[i];
		if (edge->cap > 0
			&& spfa->dist[node] + edge->cost < spfa->dist[edge->to])
		{
			spfa->dist[edge->to] = spfa->dist[node] + edge->cost;
			spfa->parent_node[edge->to] = node;
			spfa->parent_edge[edge->to] = edge;
			if (!spfa->in_queue[edge->to])
				spfa_push(spfa, tail, edge->to, graph->node_count);
		}
		i++;
	}
}

static void	spfa_find_path(t_flow_graph *graph, t_spfa *spfa) {
	int	head;
	int	tail;
	int	node;

	spfa_reset(spfa, graph->node_count);
	head = 0;
	tail = 0;
	spfa->dist[graph->source] = 0;
	spfa_push(spfa, &tail, graph->source, graph->node_count);
	while (head != tail)
	{
		node = spfa_pop(spfa, &head, graph->node_count);
		spfa_relax_edges(graph, spfa, node, &tail);
	}
}

static void	augment_path(t_flow_graph *graph, t_spfa *spfa) {
	t_flow_edge	*edge;
	int			node;

	node = graph->sink;
	while (node != graph->source)
	{
		edge = spfa->parent_edge[node];
		edge->cap--;
		edge->rev->cap++;
		node = edge->from;
	}
}

int	min_cost_augment(t_flow_graph *graph, t_spfa *spfa, int *found) {
	*found = 0;
	spfa_find_path(graph, spfa);
	if (spfa->parent_edge[graph->sink])
	{
		augment_path(graph, spfa);
		*found = 1;
	}
	return (ERR_NO_ERROR);
}