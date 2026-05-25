/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_insert.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:27:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/04/21 13:03:14 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

size_t		hash_function(const char *key, size_t capacity);
static int	need_rehash(t_hashmap *map);
static int	prepare_new_node(t_hashnode *node, const char *key, void *value);
static void	swap_nodes(t_hashnode *a, t_hashnode *b);
int			robinhood_probe(t_hashmap *map, t_hashnode new_node);
int			hashmap_rehash(t_hashmap *map);

/***
 * @brief Insert a key-value pair into the hashmap
 * @param map the hashmap to insert into
 * @param key the key to insert
 * @param value the value to insert
 * @return 0 on success, -1 on failure
*/
int	hashmap_insert(t_hashmap *map, const char *key, void *value)
{
	t_hashnode	new_node;

	if (!map || !key)
		return (-1);
	if (need_rehash(map) && hashmap_rehash(map) == -1)
		return (-1);
	if (prepare_new_node(&new_node, key, value) == -1)
		return (-1);
	return (robinhood_probe(map, new_node));
}

/***
 * @brief Check if the hashmap needs to be rehashed
 */
static int	need_rehash(t_hashmap *map)
{
	return (map->count * 10 >= map->capacity * 7);
}

/***
 * @brief Prepare a new node for insertion
 */
static int	prepare_new_node(t_hashnode *node, const char *key, void *value)
{
	node->key = ft_strdup(key);
	if (!node->key)
		return (-1);
	node->value = value;
	node->distance = 0;
	return (0);
}

/***
 * @brief Swap two nodes
 */
static void	swap_nodes(t_hashnode *a, t_hashnode *b)
{
	t_hashnode	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/***
 * @brief Insert a new node into the hashmap using Robin Hood hashing
 * @param map the hashmap to insert into
 * @param new_node the new node to insert
 * @return 0 on success, -1 on failure
 */
int	robinhood_probe(t_hashmap *map, t_hashnode new_node)
{
	size_t	index;

	index = hash_function(new_node.key, map->capacity);
	while (map->nodes[index].key)
	{
		if (ft_strcmp(map->nodes[index].key, new_node.key) == 0)
		{
			free(map->nodes[index].key);
			map->nodes[index] = new_node;
			return (0);
		}
		if (new_node.distance > map->nodes[index].distance)
		{
			swap_nodes(&new_node, &map->nodes[index]);
		}
		index = (index + 1) % map->capacity;
		new_node.distance++;
	}
	map->nodes[index] = new_node;
	map->count++;
	return (0);
}
