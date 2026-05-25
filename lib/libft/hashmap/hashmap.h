/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:49:32 by kearmand          #+#    #+#             */
/*   Updated: 2025/04/21 13:42:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_H
# define HASHMAP_H

/*
** Hashmap implementation using open addressing with Robin Hood hashing
** and backward shift deletion for removals. Provides efficient insertions,
** lookups, and deletions with good cache performance.
** Hashfonction is based on FNV-1a hash function.
*/

# include <stddef.h>
# include <string.h>

typedef struct s_hashnode
{
	char				*key;
	void				*value;
	size_t				distance;
}	t_hashnode;

typedef struct s_hashmap
{
	t_hashnode			*nodes;
	size_t				capacity;
	size_t				count;
}	t_hashmap;

/***
 * Hashmap functions
 */

t_hashmap	*hashmap_create(size_t initial_capacity);
void		*hashmap_get(t_hashmap *map, const char *key);
int			hashmap_insert(t_hashmap *map, const char *key, void *value);
int			hashmap_remove(t_hashmap *map, const char *key);
void		hashmap_destroy(t_hashmap *map, void (*destroy)(void *));

#endif
