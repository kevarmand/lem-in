/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_memory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:54:03 by kearmand          #+#    #+#             */
/*   Updated: 2025/04/21 13:03:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/***
 * @brief Create a new hashmap with the specified initial capacity.
 */
t_hashmap	*hashmap_create(size_t initial_capacity)
{
	t_hashmap	*map;

	map = (t_hashmap *)malloc(sizeof(t_hashmap));
	if (!map)
		return (NULL);
	map->nodes = (t_hashnode *)ft_calloc(initial_capacity, sizeof(t_hashnode));
	if (!map->nodes)
	{
		free(map);
		return (NULL);
	}
	map->capacity = initial_capacity;
	map->count = 0;
	return (map);
}

/***
 * @brief destroy the hashmap and free all its nodes
 * @param map the hashmap to destroy
 * @param destroy a function to destroy the value of each node
 * @note if destroy is NULL, the value will not be destroyed
 */
void	hashmap_destroy(t_hashmap *map, void (*destroy)(void *))
{
	size_t	i;

	if (!map)
		return ;
	if (map->nodes)
	{
		i = 0;
		while (i < map->capacity)
		{
			if (map->nodes[i].key)
			{
				free(map->nodes[i].key);
				if (destroy && map->nodes[i].value)
					destroy(map->nodes[i].value);
			}
			i++;
		}
		free(map->nodes);
	}
	free(map);
}
