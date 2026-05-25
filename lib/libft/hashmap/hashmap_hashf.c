/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_hashf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:45:06 by kearmand          #+#    #+#             */
/*   Updated: 2025/04/21 13:03:08 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/***
 * @brief Give the hash of a key
 * @param key the key to hash
 * @param capacity the capacity of the hashmap
 * @note the hash is calculated using the FNV-1a hash function
 * @return the hash of the key
 */
size_t	hash_function(const char *key, size_t capacity)
{
	size_t		hash;
	size_t		i;

	hash = 14695981039346656037UL;
	i = 0;
	while (key[i])
	{
		hash ^= (unsigned char)key[i];
		hash *= 1099511628211UL;
		i++;
	}
	return (hash % capacity);
}
