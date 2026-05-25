/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:11:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/02/14 17:35:17 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

# include "libft.h"

typedef struct s_gnl
{
	char	buffer[BUFFER_SIZE];
	int		i;
	int		size;
}	t_gnl;

int		ft_strnchr(char *s, char c, int n);
int		ft_fill_line(char **line, t_gnl *buf, int index);

#endif
