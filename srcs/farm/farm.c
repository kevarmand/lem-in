/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:39:39 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:39:41 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "farm.h"
#include "utils.h"

static void	destroy_vector_content(t_vector *vec, void (*destroy)(void *));

void	farm_init(t_farm *farm) {
	farm->ants = 0;
	farm->start = NULL;
	farm->end = NULL;
	farm->rooms_by_name = NULL;
	farm->rooms_by_coord = NULL;
	farm->links_by_key = NULL;
	farm->error = ERR_NO_ERROR;
	farm->rooms.data = NULL;
	farm->rooms.count = 0;
	farm->rooms.capacity = 0;
	farm->links.data = NULL;
	farm->links.count = 0;
	farm->links.capacity = 0;
}

int	farm_alloc(t_farm *farm) {
	if (vector_init(&farm->rooms, 64))
		return (ERR_MALLOC);
	if (vector_init(&farm->links, 128))
		return (ERR_MALLOC);
	farm->rooms_by_name = hashmap_create(128);
	if (!farm->rooms_by_name)
		return (ERR_MALLOC);
	farm->rooms_by_coord = hashmap_create(128);
	if (!farm->rooms_by_coord)
		return (ERR_MALLOC);
	farm->links_by_key = hashmap_create(256);
	if (!farm->links_by_key)
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

void	room_destroy(void *ptr) {
	t_room	*room;

	room = (t_room *)ptr;
	if (!room)
		return ;
	vector_destroy(&room->neighbors);
	free(room->name);
	free(room);
}

void	link_destroy(void *ptr) {
	t_link	*link;

	link = (t_link *)ptr;
	if (!link)
		return ;
	free(link->raw);
	free(link);
}

void	farm_destroy(t_farm *farm) {
	destroy_vector_content(&farm->links, link_destroy);
	destroy_vector_content(&farm->rooms, room_destroy);
	if (farm->rooms_by_name)
		hashmap_destroy(farm->rooms_by_name, NULL);
	if (farm->rooms_by_coord)
		hashmap_destroy(farm->rooms_by_coord, NULL);
	if (farm->links_by_key)
		hashmap_destroy(farm->links_by_key, NULL);
	farm_init(farm);
}

int	farm_add_room(t_farm *farm, t_room *room) {
	char	*coord_key;

	coord_key = make_coord_key(room->x, room->y);
	if (!coord_key)
		return (ERR_MALLOC);
	if (hashmap_get(farm->rooms_by_name, room->name)
		|| hashmap_get(farm->rooms_by_coord, coord_key))
	{
		free(coord_key);
		return (ERR_ROOM);
	}
	if (hashmap_insert(farm->rooms_by_name, room->name, room)
		|| hashmap_insert(farm->rooms_by_coord, coord_key, room)
		|| vector_push_back(&farm->rooms, room))
	{
		free(coord_key);
		return (ERR_MALLOC);
	}
	free(coord_key);
	return (ERR_NO_ERROR);
}

int	farm_add_link(t_farm *farm, t_link *link) {
	if (hashmap_get(farm->links_by_key, link->raw))
		return (ERR_NO_ERROR);
	if (vector_push_back(&link->a->neighbors, link->b))
		return (ERR_MALLOC);
	if (vector_push_back(&link->b->neighbors, link->a))
		return (ERR_MALLOC);
	if (hashmap_insert(farm->links_by_key, link->raw, link))
		return (ERR_MALLOC);
	if (vector_push_back(&farm->links, link))
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

static void	destroy_vector_content(t_vector *vec, void (*destroy)(void *)) {
	size_t	i;

	i = 0;
	while (i < vec->count)
	{
		destroy(vec->data[i]);
		i++;
	}
	vector_destroy(vec);
}