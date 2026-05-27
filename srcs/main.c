/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:15:55 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 18:56:07 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int	main(int argc, char **argv) {
	t_farm		farm;
	t_solution	solution;
	int			err;

	(void)argv;
	if (argc != 1)
		return (error(ERR_FORMAT));
	farm_init(&farm);
	err = farm_alloc(&farm);
	if (!err)
		err = parse_farm(&farm);
	if (!err)
		err = solve_farm(&farm, &solution);
	if (!err)
		print_solution(&farm, &solution);
	if (!err)
		solution_destroy(&solution);
	farm_destroy(&farm);
	return (error(err));
}