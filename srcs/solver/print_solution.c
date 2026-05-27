/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_solution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:38:21 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/26 16:39:44 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "solver.h"
#include "libft.h"

static void	print_farm_rooms(t_farm *farm) {
	t_room	*room;
	size_t	i;

	ft_printf("%d\n", farm->ants);
	ft_printf("##start\n");
	ft_printf("%s %d %d\n", farm->start->name, farm->start->x,
		farm->start->y);
	ft_printf("##end\n");
	ft_printf("%s %d %d\n", farm->end->name, farm->end->x, farm->end->y);
	i = 0;
	while (i < farm->rooms.count)
	{
		room = farm->rooms.data[i];
		if (room != farm->start && room != farm->end)
			ft_printf("%s %d %d\n", room->name, room->x, room->y);
		i++;
	}
}

static void	print_farm_links(t_farm *farm) {
	t_link	*link;
	size_t	i;

	i = 0;
	while (i < farm->links.count)
	{
		link = farm->links.data[i];
		ft_printf("%s-%s\n", link->a->name, link->b->name);
		i++;
	}
    ft_printf("\n");
}

static void	print_farm(t_farm *farm) {
	print_farm_rooms(farm);
	print_farm_links(farm);
}

static void	print_move(int *first, int ant_id, t_room *room) {
	if (!*first)
		ft_printf(" ");
	ft_printf("L%d-%s", ant_id, room->name);
	*first = 0;
}

static int	**alloc_lanes(t_solution *solution) {
	int		**lanes;
	t_path	*path;
	int		i;

	lanes = ft_calloc(solution->path_count, sizeof(*lanes));
	if (!lanes)
		return (NULL);
	i = 0;
	while (i < solution->path_count)
	{
		path = solution->paths.data[i];
		lanes[i] = ft_calloc(path->len, sizeof(**lanes));
		if (!lanes[i])
			return (lanes);
		i++;
	}
	return (lanes);
}

static void	free_lanes(int **lanes, int count) {
	int	i;

	i = 0;
	while (i < count)
	{
		free(lanes[i]);
		i++;
	}
	free(lanes);
}

static void	move_existing_ants(t_path *path, int *lane, int *done,
	int *first) {
	t_room	*room;
	int		pos;
	int		ant_id;

	pos = path->len - 2;
	while (pos >= 0)
	{
		ant_id = lane[pos];
		if (ant_id)
		{
			lane[pos] = 0;
			room = path->rooms.data[pos + 2];
			print_move(first, ant_id, room);
			if (pos == path->len - 2)
				(*done)++;
			else
				lane[pos + 1] = ant_id;
		}
		pos--;
	}
}

static void	send_new_ant(t_path *path, int *lane, int *next_ant, int *first) {
	t_room	*room;

	room = path->rooms.data[1];
	print_move(first, *next_ant, room);
	if (path->len == 1)
		;
	else
		lane[0] = *next_ant;
	(*next_ant)++;
}

static void	run_one_round(t_solution *solution, int **lanes, int *state) {
	t_path	*path;
	int		i;
	int		first;

	first = 1;
	i = 0;
	while (i < solution->path_count)
	{
		path = solution->paths.data[i];
		move_existing_ants(path, lanes[i], &state[1], &first);
		i++;
	}
	i = 0;
	while (i < solution->path_count)
	{
		path = solution->paths.data[i];
		if (path->ants > state[i + 3])
		{
			send_new_ant(path, lanes[i], &state[0], &first);
			state[i + 3]++;
			if (path->len == 1)
				state[1]++;
		}
		i++;
	}
	ft_printf("\n");
}

static void	print_moves(t_farm *farm, t_solution *solution) {
	int	**lanes;
	int	*state;

	lanes = alloc_lanes(solution);
	state = ft_calloc(solution->path_count + 3, sizeof(*state));
	if (!lanes || !state)
	{
		free_lanes(lanes, solution->path_count);
		free(state);
		return ;
	}
	state[0] = 1;
	state[1] = 0;
	while (state[1] < farm->ants)
		run_one_round(solution, lanes, state);
	free_lanes(lanes, solution->path_count);
	free(state);
}

void	print_solution(t_farm *farm, t_solution *solution) {
	print_farm(farm);
	print_moves(farm, solution);
}