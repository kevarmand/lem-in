/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_remove.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:14:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/04/21 13:03:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

size_t		hash_function(const char *key, size_t capacity);
static void	compact_after_removal(t_hashmap *map, size_t index);

/***
 * @brief Remove a key-value pair from the hashmap
 * @param map The hashmap to remove from
 * @param key The key to remove
 * @return 0 on success, -1 if not found
 */
int	hashmap_remove(t_hashmap *map, const char *key)
{
	size_t	index;

	if (!map || !key)
		return (-1);
	index = hash_function(key, map->capacity);
	while (map->nodes[index].key)
	{
		if (ft_strcmp(map->nodes[index].key, key) == 0)
		{
			free(map->nodes[index].key);
			map->nodes[index].key = NULL;
			map->nodes[index].value = NULL;
			map->nodes[index].distance = 0;
			map->count--;
			compact_after_removal(map, index);
			return (0);
		}
		index = (index + 1) % map->capacity;
	}
	return (-1);
}

/***
 * @brief Compact the cluster after a removal to maintain probing invariants
 * @param map The hashmap to compact
 * @param index The index where the deletion happened
 */
static void	compact_after_removal(t_hashmap *map, size_t index)
{
	size_t	next;

	next = (index + 1) % map->capacity;
	while (map->nodes[next].key && map->nodes[next].distance > 0)
	{
		map->nodes[next].distance--;
		map->nodes[index] = map->nodes[next];
		map->nodes[next].key = NULL;
		map->nodes[next].value = NULL;
		map->nodes[next].distance = 0;
		index = next;
		next = (next + 1) % map->capacity;
	}
}
