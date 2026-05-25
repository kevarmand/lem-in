/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:37:24 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:37:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int		custom_atoi(const char *str, int *out);
char	*join_three(const char *a, const char *b, const char *c);
char	*make_coord_key(int x, int y);
char	*make_link_key(const char *a, const char *b);

#endif