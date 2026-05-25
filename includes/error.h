/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:37:15 by kearmand          #+#    #+#             */
/*   Updated: 2026/05/25 13:37:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

enum e_error
{
	ERR_NO_ERROR,
	ERR_MALLOC,
	ERR_FILE,
	ERR_FORMAT,
	ERR_LINK,
	ERR_ROOM,
	ERR_START_END,
	ERR_ANTS,
	ERR_MOVE,
	ERR_UNKNOWN
};

int	error(int error_code);

#endif