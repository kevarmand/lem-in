/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer_destroy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:45:25 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 17:12:30 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "visualizer.h"

static void	destroy_step(void *ptr);

void	visualizer_destroy(t_visu *visu) {
	size_t	i;

	i = 0;
	while (i < visu->anim.steps.count)
	{
		destroy_step(visu->anim.steps.data[i]);
		i++;
	}
	vector_destroy(&visu->anim.steps);
	free(visu->anim.ants);
	free(visu->anim.active_ants);
	free(visu->anim.active_pos);
	free(visu->anim.ant_status);
	vector_destroy(&visu->paths);
	vector_destroy(&visu->layout.nodes);
}

static void	destroy_step(void *ptr) {
	t_step	*step;
	size_t	i;

	step = (t_step *)ptr;
	if (!step)
		return ;
	i = 0;
	while (i < step->moves.count)
	{
		free(step->moves.data[i]);
		i++;
	}
	vector_destroy(&step->moves);
	free(step);
}