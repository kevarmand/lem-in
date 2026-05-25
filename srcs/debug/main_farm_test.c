/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_farm_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:48:35 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:48:43 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int	main(int argc, char **argv) {
	t_farm	farm;
	int		err;

	(void)argv;
	if (argc != 1)
		return (error(ERR_FORMAT));
	farm_init(&farm);
	err = farm_alloc(&farm);
	if (!err)
		err = parse_farm(&farm);
	if (!err)
		dump_farm(&farm);
	farm_destroy(&farm);
	return (error(err));
}