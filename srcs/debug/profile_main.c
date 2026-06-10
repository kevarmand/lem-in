/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profile_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:20:08 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/02 16:20:09 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include "lem_in.h"

static long	profile_now_us(void) {
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000L) + tv.tv_usec);
}

static void	profile_print_ms(char *name, long us) {
	fprintf(stderr, "%-12s %8.3f ms\n", name, (double)us / 1000.0);
}

static void	profile_print_infos(t_farm *farm, t_solution *solution, int solved) {
	fprintf(stderr, "\n--- profile infos ---\n");
	fprintf(stderr, "ants        %d\n", farm->ants);
	fprintf(stderr, "rooms       %zu\n", farm->rooms.count);
	fprintf(stderr, "links       %zu\n", farm->links.count);
	if (solved)
	{
		fprintf(stderr, "paths       %d\n", solution->path_count);
		fprintf(stderr, "rounds      %d\n", solution->rounds);
		fprintf(stderr, "total_len   %d\n", solution->total_len);
	}
}

static int	profile_run(void) {
	t_farm		farm;
	t_solution	solution;
	long		t0;
	long		t1;
	long		t2;
	long		t3;
	long		t4;
	int			err;
	int			solved;

	solved = 0;
	farm_init(&farm);
	t0 = profile_now_us();
	err = farm_alloc(&farm);
	t1 = profile_now_us();
	if (!err)
		err = parse_farm(&farm);
	t2 = profile_now_us();
	if (!err)
	{
		err = solve_farm(&farm, &solution);
		if (!err)
			solved = 1;
	}
	t3 = profile_now_us();
	if (!err)
		print_solution(&farm, &solution);
	t4 = profile_now_us();
	if (solved)
		solution_destroy(&solution);
	profile_print_infos(&farm, &solution, solved);
	fprintf(stderr, "\n--- profile timing ---\n");
	profile_print_ms("alloc", t1 - t0);
	profile_print_ms("parse", t2 - t1);
	profile_print_ms("solve", t3 - t2);
	profile_print_ms("print", t4 - t3);
	profile_print_ms("total", t4 - t0);
	farm_destroy(&farm);
	return (error(err));
}

int	main(int argc, char **argv) {
	(void)argv;
	if (argc != 1)
		return (error(ERR_FORMAT));
	return (profile_run());
}