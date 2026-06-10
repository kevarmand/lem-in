/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solution.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:33:23 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 11:40:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdlib.h>
#include "solver.h"
#include "libft.h"

int	solution_init(t_solution *solution, int capacity) {
	solution->path_count = 0;
	solution->rounds = 0;
	solution->total_len = 0;
	solution->paths.data = NULL;
	solution->paths.count = 0;
	solution->paths.capacity = 0;
	if (capacity < 1)
		capacity = 1;
	if (vector_init(&solution->paths, capacity))
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

t_path	*path_create(int capacity) {
	t_path	*path;

	path = malloc(sizeof(*path));
	if (!path)
		return (NULL);
	path->len = 0;
	path->ants = 0;
	path->rooms.data = NULL;
	path->rooms.count = 0;
	path->rooms.capacity = 0;
	if (capacity < 1)
		capacity = 1;
	if (vector_init(&path->rooms, capacity))
	{
		free(path);
		return (NULL);
	}
	return (path);
}

int	path_push_room(t_path *path, t_room *room) {
	if (vector_push_back(&path->rooms, room))
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

int	solution_add_path(t_solution *solution, t_path *path) {
	if (vector_push_back(&solution->paths, path))
		return (ERR_MALLOC);
	solution->path_count++;
	solution->total_len += path->len;
	return (ERR_NO_ERROR);
}

void	path_destroy(void *ptr) {
	t_path	*path;

	path = (t_path *)ptr;
	if (!path)
		return ;
	vector_destroy(&path->rooms);
	free(path);
}

void	solution_destroy(t_solution *solution) {
	size_t	i;

	i = 0;
	while (i < solution->paths.count)
	{
		path_destroy(solution->paths.data[i]);
		i++;
	}
	vector_destroy(&solution->paths);
	solution->path_count = 0;
	solution->rounds = 0;
	solution->total_len = 0;
}

static int	clone_path(t_path **dst, t_path *src) {
	t_path	*path;
	size_t	i;

	path = path_create((int)src->rooms.count);
	if (!path)
		return (ERR_MALLOC);
	i = 0;
	while (i < src->rooms.count)
	{
		if (path_push_room(path, src->rooms.data[i]))
		{
			path_destroy(path);
			return (ERR_MALLOC);
		}
		i++;
	}
	path->len = src->len;
	path->ants = src->ants;
	*dst = path;
	return (ERR_NO_ERROR);
}

int	solution_clone(t_solution *dst, t_solution *src) {
	t_path	*path;
	size_t	i;
	int		err;

	err = solution_init(dst, src->path_count);
	if (err)
		return (err);
	i = 0;
	while (i < src->paths.count)
	{
		path = NULL;
		err = clone_path(&path, src->paths.data[i]);
		if (!err)
			err = solution_add_path(dst, path);
		if (err)
		{
			if (path)
				path_destroy(path);
			solution_destroy(dst);
			return (err);
		}
		i++;
	}
	dst->rounds = src->rounds;
	dst->total_len = src->total_len;
	return (ERR_NO_ERROR);
}