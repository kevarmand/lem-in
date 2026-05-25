/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver_stub.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:49:14 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:49:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "solver.h"

int	solve_farm(t_farm *farm, t_solution *solution) {
	(void)farm;
	solution->path_count = 0;
	solution->rounds = 0;
	vector_init(&solution->paths, 1);
	return (ERR_NO_ERROR);
}

void	solution_destroy(t_solution *solution) {
	vector_destroy(&solution->paths);
}

void	print_solution(t_farm *farm, t_solution *solution) {
	(void)solution;
	printf("%d\n", farm->ants);
}