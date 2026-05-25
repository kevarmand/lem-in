/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:14:22 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:38:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_H
# define SOLVER_H

# include "farm.h"

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
}	t_solution;

typedef struct s_solver
{
	t_farm		*farm;
	int			*visited;
	int			*distance;
	t_room		**prev;
	t_solution	best;
}	t_solver;

int		solve_farm(t_farm *farm, t_solution *solution);
void	solution_destroy(t_solution *solution);
void	print_solution(t_farm *farm, t_solution *solution);

#endif