/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:24:14 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/01 14:04:32 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	vector_init(t_vector *vec, size_t initial_capacity)
{
	vec->data = (void **)malloc(sizeof(void *) * initial_capacity);
	if (!vec->data)
		return (-1);
	vec->count = 0;
	vec->capacity = initial_capacity;
	return (0);
}

void	vector_destroy(t_vector *vec)
{
	if (vec->data)
		free(vec->data);
	vec->data = NULL;
	vec->count = 0;
	vec->capacity = 0;
}

int	vector_push_back(t_vector *vec, void *elem)
{
	void	**new_data;
	size_t	new_capacity;
	size_t	i;

	if (vec->count >= vec->capacity)
	{
		new_capacity = vec->capacity * 2;
		new_data = malloc(new_capacity * sizeof(void *));
		if (!new_data)
			return (-1);
		i = 0;
		while (i < vec->count)
		{
			new_data[i] = vec->data[i];
			i++;
		}
		free(vec->data);
		vec->data = new_data;
		vec->capacity = new_capacity;
	}
	vec->data[vec->count++] = elem;
	return (0);
}
/***
 * @brief Remove an element from the vector at the specified index
 * @param vec The vector to remove from
 * @param index The index of the element to remove
 */
void vector_remove(t_vector *vec, size_t index)
{
	size_t i;

	if (!vec || index >= vec->count)
		return;
	i = index;
	while (i < vec->count - 1)
	{
		vec->data[i] = vec->data[i + 1];
		i++;
	}
	vec->count--;
}
