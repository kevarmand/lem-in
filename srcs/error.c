/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:39:03 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/10 12:05:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "error.h"

int	error(int error_code) {
	if (error_code == ERR_NO_ERROR)
		return (0);
	if (error_code == ERR_MALLOC)
		write(2, "Error: malloc failed\n", 21);
	else if (error_code == ERR_FILE)
		write(2, "Error: file error\n", 18);
	else if (error_code == ERR_FORMAT)
		write(2, "Error: invalid format\n", 22);
	else if (error_code == ERR_LINK)
		write(2, "Error: invalid link\n", 20);
	else if (error_code == ERR_ROOM)
		write(2, "Error: invalid room\n", 20);
	else if (error_code == ERR_START_END)
		write(2, "Error: invalid start or end\n", 28);
	else if (error_code == ERR_ANTS)
		write(2, "Error: invalid number of ants\n", 30);
	else if (error_code == ERR_MOVE)
		write(2, "Error: invalid move\n", 20);
	else if (error_code == ERR_NO_PATH)
		write(2, "Error: no path found\n", 22);
	else
		write(2, "Error: unknown error\n", 21);
	return (1);
}