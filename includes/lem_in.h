/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:15:14 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:38:41 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <string.h>
# include <stdint.h>

# include "error.h"
# include "libft.h"
# include "utils.h"
# include "farm.h"
# include "parsing.h"
# include "solver.h"

void	dump_farm(t_farm *farm);

#endif