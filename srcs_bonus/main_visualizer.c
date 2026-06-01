/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_visualizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:45:59 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/01 14:19:53 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visualizer.h"
#include "error.h"
#include "libft.h"

static int	parse_visualizer_args(int argc, char **argv, int *reorganize);

int	main(int argc, char **argv) {
	t_farm	farm;
	t_visu	visu;
	int		err;
	int		reorganize;

	ft_bzero(&visu, sizeof(visu));
	farm_init(&farm);
	err = parse_visualizer_args(argc, argv, &reorganize);
	if (!err)
		err = farm_alloc(&farm);
	if (!err)
		err = parse_visu_input(&farm, &visu);
	if (!err && reorganize)
		err = visu_reorganize_rooms(&visu);
	if (!err)
		err = launch_visualizer(&visu);
	visualizer_destroy(&visu);
	farm_destroy(&farm);
	return (error(err));
}

static int	parse_visualizer_args(int argc, char **argv, int *reorganize) {
	*reorganize = 0;
	if (argc == 1)
		return (ERR_NO_ERROR);
	if (argc == 2 && !ft_strcmp(argv[1], "-r"))
	{
		*reorganize = 1;
		return (ERR_NO_ERROR);
	}
	return (ERR_FORMAT);
}