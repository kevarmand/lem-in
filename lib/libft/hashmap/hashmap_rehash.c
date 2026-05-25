/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_rehash.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:07:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/04/21 13:03:20 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int			robinhood_probe(t_hashmap *map, t_hashnode new_node);
size_t		hash_function(const char *key, size_t capacity);
static int	allocate_new_table(t_hashmap *map);
static void	reinsert_old_nodes(t_hashmap *map, t_hashnode *old_nodes,
				size_t old_capacity);

/***
 * @brief Rehash the hashmap when the load factor is too high.
 * @param map The hashmap to rehash.
 * @return 0 on success, -1 on failure.
 */
int	hashmap_rehash(t_hashmap *map)
{
	t_hashnode	*old_nodes;
	size_t		old_capacity;

	old_nodes = map->nodes;
	old_capacity = map->capacity;
	if (allocate_new_table(map) == -1)
	{
		map->nodes = old_nodes;
		return (-1);
	}
	reinsert_old_nodes(map, old_nodes, old_capacity);
	free(old_nodes);
	return (0);
}

/***
 * @brief Allocate a new table with double the previous capacity.
 * @param map The hashmap to update.
 * @return 0 on success, -1 on failure.
 */
static int	allocate_new_table(t_hashmap *map)
{
	t_hashnode	*new_nodes;

	new_nodes = (t_hashnode *)ft_calloc(map->capacity * 2, sizeof(t_hashnode));
	if (!new_nodes)
		return (-1);
	map->nodes = new_nodes;
	map->capacity *= 2;
	map->count = 0;
	return (0);
}

/***
 * @brief Reinsert old nodes into the new table.
 * @param map The new hashmap.
 * @param old_nodes The old array of nodes.
 * @param old_capacity The capacity of the old array.
 */
static void	reinsert_old_nodes(t_hashmap *map, t_hashnode *old_nodes,
	size_t old_capacity)
{
	size_t		i;
	t_hashnode	new_node;

	i = 0;
	while (i < old_capacity)
	{
		if (old_nodes[i].key)
		{
			new_node = old_nodes[i];
			new_node.distance = 0;
			robinhood_probe(map, new_node);
		}
		i++;
	}
}
