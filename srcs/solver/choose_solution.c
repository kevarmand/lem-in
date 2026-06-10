/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_solution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:25:02 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 12:05:40 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "solver.h"

static long	path_capacity(t_path *path, int rounds) {
	if (rounds < path->len)
		return (0);
	return ((long)rounds - path->len + 1);
}

static long	solution_capacity(t_solution *solution, int rounds) {
	t_path	*path;
	long	capacity;
	size_t	i;

	capacity = 0;
	i = 0;
	while (i < solution->paths.count)
	{
		path = solution->paths.data[i];
		capacity += path_capacity(path, rounds);
		i++;
	}
	return (capacity);
}

static int	max_path_len(t_solution *solution) {
	t_path	*path;
	int		max;
	size_t	i;

	max = 0;
	i = 0;
	while (i < solution->paths.count)
	{
		path = solution->paths.data[i];
		if (path->len > max)
			max = path->len;
		i++;
	}
	return (max);
}

int	solution_rounds(t_solution *solution, int ants) {
	int	low;
	int	high;
	int	mid;

	low = 0;
	high = max_path_len(solution) + ants;
	while (low < high)
	{
		mid = low + (high - low) / 2;
		if (solution_capacity(solution, mid) >= ants)
			high = mid;
		else
			low = mid + 1;
	}
	return (low);
}

static int	is_better_solution(t_solution *candidate, t_solution *best) {
	if (best->path_count == 0)
		return (1);
	if (candidate->rounds < best->rounds)
		return (1);
	if (candidate->rounds > best->rounds)
		return (0);
	if (candidate->path_count < best->path_count)
		return (1);
	if (candidate->path_count > best->path_count)
		return (0);
	return (candidate->total_len < best->total_len);
}

void	assign_ants(t_solution *solution, int ants) {
	t_path	*path;
	t_path	*best;
	int		i;
	int		best_score;
	int		score;

	while (ants > 0)
	{
		best = solution->paths.data[0];
		best_score = best->len + best->ants;
		i = 1;
		while (i < solution->path_count)
		{
			path = solution->paths.data[i];
			score = path->len + path->ants;
			if (score < best_score)
			{
				best = path;
				best_score = score;
			}
			i++;
		}
		best->ants++;
		ants--;
	}
}

int	choose_solution(t_farm *farm, t_vector *bests, t_solution *solution) {
	t_solution	*candidate;
	t_solution	current_best;
	size_t		i;
	int			err;

	err = solution_init(&current_best, 1);
	if (err)
		return (err);
	i = 0;
	while (i < bests->count)
	{
		candidate = bests->data[i];
		candidate->rounds = solution_rounds(candidate, farm->ants);
		if (is_better_solution(candidate, &current_best))
		{
			solution_destroy(&current_best);
			err = solution_clone(&current_best, candidate);
			if (err)
				return (err);
		}
		i++;
	}
	if (current_best.path_count == 0)
		return (solution_destroy(&current_best), ERR_NO_PATH);
	err = solution_clone(solution, &current_best);
	solution_destroy(&current_best);
	if (err)
		return (err);
	assign_ants(solution, farm->ants);
	solution->rounds = solution_rounds(solution, farm->ants);
	return (ERR_NO_ERROR);
}