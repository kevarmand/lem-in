/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:12:31 by kearmand          #+#    #+#             */
/*   Updated: 2025/04/21 13:03:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		hash_function(const char *key, size_t capacity);

/***
 * @brief Retrieve a value by its key from the hashmap.
 * @param map The hashmap to search.
 * @param key The key to look for.
 * @return The value associated with the key, or NULL if not found.
 */
void	*hashmap_get(t_hashmap *map, const char *key)
{
	size_t	index;

	if (!map || !key)
		return (NULL);
	index = hash_function(key, map->capacity);
	while (map->nodes[index].key)
	{
		if (ft_strcmp(map->nodes[index].key, key) == 0)
			return (map->nodes[index].value);
		index = (index + 1) % map->capacity;
	}
	return (NULL);
}
