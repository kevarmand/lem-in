/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:25:39 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 16:25:41 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "solver.h"
#include "libft.h"

static void	destroy_bests(t_vector *bests)
{
	t_solution	*solution;
	size_t		i;

	i = 0;
	while (i < bests->count)
	{
		solution = bests->data[i];
		solution_destroy(solution);
		free(solution);
		i++;
	}
	vector_destroy(bests);
}

static int	store_current_best(t_solver *solver)
{
	t_solution	*solution;
	int			err;

	solution = malloc(sizeof(*solution));
	if (!solution)
		return (ERR_MALLOC);
	err = extract_solution(&solver->flow, solution);
	if (!err)
		err = vector_push_back(&solver->bests, solution);
	if (err)
	{
		if (solution)
		{
			solution_destroy(solution);
			free(solution);
		}
		return (err);
	}
	return (ERR_NO_ERROR);
}

static int	build_all_bests(t_solver *solver)
{
	int	found;
	int	flow_count;
	int	err;

	flow_count = 0;
	found = 1;
	while (found && flow_count < solver->farm->ants)
	{
		err = min_cost_augment(&solver->flow, &found);
		if (err)
			return (err);
		if (found)
		{
			flow_count++;
			err = store_current_best(solver);
			if (err)
				return (err);
		}
	}
	return (ERR_NO_ERROR);
}

int	solve_farm(t_farm *farm, t_solution *solution)
{
	t_solver	solver;
	int			err;

	solver.farm = farm;
	err = vector_init(&solver.bests, 8);
	if (err)
		return (ERR_MALLOC);
	err = flow_graph_init(&solver.flow, farm);
	if (!err)
		err = build_all_bests(&solver);
	if (!err)
		err = choose_solution(farm, &solver.bests, solution);
	flow_graph_destroy(&solver.flow);
	destroy_bests(&solver.bests);
	return (err);
}