/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump_farm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:46:31 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:46:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "lem_in.h"

static void	dump_rooms(t_farm *farm);
static void	dump_links(t_farm *farm);
static void	dump_room_neighbors(t_room *room);

void	dump_farm(t_farm *farm) {
	printf("=== FARM ===\n");
	printf("ants: %d\n", farm->ants);
	if (farm->start)
		printf("start: #%d %s (%d,%d)\n",
			farm->start->id, farm->start->name, farm->start->x, farm->start->y);
	else
		printf("start: NULL\n");
	if (farm->end)
		printf("end: #%d %s (%d,%d)\n",
			farm->end->id, farm->end->name, farm->end->x, farm->end->y);
	else
		printf("end: NULL\n");
	printf("rooms: %zu\n", farm->rooms.count);
	printf("links: %zu\n", farm->links.count);
	dump_rooms(farm);
	dump_links(farm);
}

static void	dump_rooms(t_farm *farm) {
	t_room	*room;
	size_t	i;

	printf("\n--- ROOMS ---\n");
	i = 0;
	while (i < farm->rooms.count)
	{
		room = (t_room *)farm->rooms.data[i];
		printf("#%d %s (%d,%d)", room->id, room->name, room->x, room->y);
		if (room == farm->start)
			printf(" [START]");
		if (room == farm->end)
			printf(" [END]");
		printf("\n");
		dump_room_neighbors(room);
		i++;
	}
}

static void	dump_room_neighbors(t_room *room) {
	t_room	*neighbor;
	size_t	i;

	printf("    neighbors:");
	i = 0;
	while (i < room->neighbors.count)
	{
		neighbor = (t_room *)room->neighbors.data[i];
		printf(" %s", neighbor->name);
		i++;
	}
	printf("\n");
}

static void	dump_links(t_farm *farm) {
	t_link	*link;
	size_t	i;

	printf("\n--- LINKS ---\n");
	i = 0;
	while (i < farm->links.count)
	{
		link = (t_link *)farm->links.data[i];
		printf("%s : %s <-> %s\n", link->raw, link->a->name, link->b->name);
		i++;
	}
}