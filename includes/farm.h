/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:12:38 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:37:46 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FARM_H
# define FARM_H

# include "libft.h"
# include "error.h"

typedef struct s_room
{
	int			id;
	char		*name;
	int			x;
	int			y;
	t_vector	neighbors;
}	t_room;

typedef struct s_link
{
	t_room		*a;
	t_room		*b;
	char		*raw;
}	t_link;

typedef struct s_farm
{
	int			ants;
	t_room		*start;
	t_room		*end;
	t_hashmap	*rooms_by_name;
	t_hashmap	*rooms_by_coord;
	t_hashmap	*links_by_key;
	t_vector	rooms;
	t_vector	links;
	int			error;
}	t_farm;

void	farm_init(t_farm *farm);
int		farm_alloc(t_farm *farm);
void	farm_destroy(t_farm *farm);
void	room_destroy(void *ptr);
void	link_destroy(void *ptr);
int		farm_add_room(t_farm *farm, t_room *room);
int		farm_add_link(t_farm *farm, t_link *link);

#endif