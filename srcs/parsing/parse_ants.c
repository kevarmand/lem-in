/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_amts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:40:49 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:40:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parse_ants(int *err, char **line, t_farm *farm) {
	int	ants;

	if (*err || !*line || farm->ants > 0)
		return (0);
	if (parse_is_comment(*line))
	{
		parse_consume_line(line);
		return (1);
	}
	parse_strip_newline(*line);
	if (custom_atoi(*line, &ants) || ants <= 0)
		*err = ERR_ANTS;
	else
		farm->ants = ants;
	parse_consume_line(line);
	return (1);
}