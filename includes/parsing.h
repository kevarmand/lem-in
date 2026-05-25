/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:14:07 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:37:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "farm.h"
# include "utils.h"

# define PARSE_CMD_NONE 0
# define PARSE_CMD_START 1
# define PARSE_CMD_END 2

typedef struct s_room_parse
{
	char	*name;
	char	*x_str;
	char	*y_str;
	int		x;
	int		y;
}	t_room_parse;

typedef struct s_link_parse
{
	char	*name_a;
	char	*name_b;
	t_room	*room_a;
	t_room	*room_b;
	char	*key;
}	t_link_parse;

int		parse_farm(t_farm *farm);
int		parse_ants(int *err, char **line, t_farm *farm);
int		parse_rooms(int *err, char **line, t_farm *farm, int *cmd);
int		parse_links(int *err, char **line, t_farm *farm);

int		parse_is_start_cmd(char *line);
int		parse_is_end_cmd(char *line);
int		parse_is_comment(char *line);
int		parse_count_char(char *line, char c);
void	parse_strip_newline(char *line);
void	parse_consume_line(char **line);
int		parse_check_farm(t_farm *farm);

#endif