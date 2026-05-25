/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_visualizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:45:59 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 14:35:15 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"
#include "error.h"
#include "libft.h"

int	main(int argc, char **argv)
{
	t_farm	farm;
	t_visu	visu;
	int		err;

	(void)argv;
	if (argc != 1)
		return (error(ERR_FORMAT));
	ft_bzero(&visu, sizeof(visu));
	farm_init(&farm);
	err = farm_alloc(&farm);
	if (!err)
		err = parse_visu_input(&farm, &visu);
	if (!err)
		err = launch_visualizer(&visu);
	visualizer_destroy(&visu);
	farm_destroy(&farm);
	return (error(err));
}