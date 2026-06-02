/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_solution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:38:21 by kearmand          #+#    #+#             */
/*   Updated: 2026/06/02 16:34:42 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "solver.h"

#define OUTBUF_SIZE 65536

typedef struct s_outbuf
{
	char	data[OUTBUF_SIZE];
	size_t	len;
	int		error;
}	t_outbuf;

typedef struct s_print_ctx
{
	t_outbuf	*out;
	t_solution	*solution;
	int			**lanes;
	int			*lane_data;
	int			*state;
}	t_print_ctx;

static int	out_flush(t_outbuf *out) {
	size_t	done;
	ssize_t	ret;

	if (out->error)
		return (out->error);
	done = 0;
	while (done < out->len)
	{
		ret = write(1, out->data + done, out->len - done);
		if (ret <= 0)
		{
			out->error = ERR_FILE;
			return (ERR_FILE);
		}
		done += ret;
	}
	out->len = 0;
	return (ERR_NO_ERROR);
}

static int	out_char(t_outbuf *out, char c) {
	if (out->error)
		return (out->error);
	if (out->len == OUTBUF_SIZE && out_flush(out))
		return (out->error);
	out->data[out->len] = c;
	out->len++;
	return (ERR_NO_ERROR);
}

static int	out_str(t_outbuf *out, const char *str) {
	while (*str)
	{
		if (out_char(out, *str))
			return (out->error);
		str++;
	}
	return (ERR_NO_ERROR);
}

static int	out_int(t_outbuf *out, int value) {
	char	tmp[20];
	long	nb;
	int		i;

	nb = value;
	if (nb == 0)
		return (out_char(out, '0'));
	if (nb < 0)
	{
		if (out_char(out, '-'))
			return (out->error);
		nb = -nb;
	}
	i = 0;
	while (nb > 0)
	{
		tmp[i] = '0' + (nb % 10);
		nb /= 10;
		i++;
	}
	while (i > 0)
	{
		i--;
		if (out_char(out, tmp[i]))
			return (out->error);
	}
	return (ERR_NO_ERROR);
}

static int	print_room_line(t_outbuf *out, t_room *room) {
	if (out_str(out, room->name))
		return (out->error);
	if (out_char(out, ' '))
		return (out->error);
	if (out_int(out, room->x))
		return (out->error);
	if (out_char(out, ' '))
		return (out->error);
	if (out_int(out, room->y))
		return (out->error);
	return (out_char(out, '\n'));
}

static int	print_farm_rooms(t_outbuf *out, t_farm *farm) {
	t_room	*room;
	size_t	i;

	if (out_int(out, farm->ants))
		return (out->error);
	if (out_char(out, '\n'))
		return (out->error);
	if (out_str(out, "##start\n"))
		return (out->error);
	if (print_room_line(out, farm->start))
		return (out->error);
	if (out_str(out, "##end\n"))
		return (out->error);
	if (print_room_line(out, farm->end))
		return (out->error);
	i = 0;
	while (i < farm->rooms.count)
	{
		room = farm->rooms.data[i];
		if (room != farm->start && room != farm->end)
		{
			if (print_room_line(out, room))
				return (out->error);
		}
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	print_farm_links(t_outbuf *out, t_farm *farm) {
	t_link	*link;
	size_t	i;

	i = 0;
	while (i < farm->links.count)
	{
		link = farm->links.data[i];
		if (out_str(out, link->a->name))
			return (out->error);
		if (out_char(out, '-'))
			return (out->error);
		if (out_str(out, link->b->name))
			return (out->error);
		if (out_char(out, '\n'))
			return (out->error);
		i++;
	}
	return (out_char(out, '\n'));
}

static int	print_farm(t_outbuf *out, t_farm *farm) {
	if (print_farm_rooms(out, farm))
		return (out->error);
	return (print_farm_links(out, farm));
}

static void	print_ctx_init(t_print_ctx *ctx, t_outbuf *out,
	t_solution *solution) {
	ctx->out = out;
	ctx->solution = solution;
	ctx->lanes = NULL;
	ctx->lane_data = NULL;
	ctx->state = NULL;
}

static void	print_ctx_destroy(t_print_ctx *ctx) {
	free(ctx->lanes);
	free(ctx->lane_data);
	free(ctx->state);
	ctx->lanes = NULL;
	ctx->lane_data = NULL;
	ctx->state = NULL;
}

static void	zero_ints(int *array, int count) {
	int	i;

	i = 0;
	while (i < count)
	{
		array[i] = 0;
		i++;
	}
}

static int	alloc_print_ctx(t_print_ctx *ctx) {
	t_path	*path;
	int		total_len;
	int		offset;
	int		i;

	if (ctx->solution->path_count <= 0)
		return (ERR_LINK);
	total_len = ctx->solution->total_len;
	if (total_len <= 0)
		total_len = 1;
	ctx->lanes = malloc(sizeof(*ctx->lanes) * ctx->solution->path_count);
	ctx->lane_data = malloc(sizeof(*ctx->lane_data) * total_len);
	ctx->state = malloc(sizeof(*ctx->state) * (ctx->solution->path_count + 3));
	if (!ctx->lanes || !ctx->lane_data || !ctx->state)
	{
		print_ctx_destroy(ctx);
		return (ERR_MALLOC);
	}
	zero_ints(ctx->lane_data, total_len);
	zero_ints(ctx->state, ctx->solution->path_count + 3);
	offset = 0;
	i = 0;
	while (i < ctx->solution->path_count)
	{
		path = ctx->solution->paths.data[i];
		ctx->lanes[i] = ctx->lane_data + offset;
		offset += path->len;
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	print_move(t_outbuf *out, int ant_id, t_room *room, int *first) {
	if (!*first && out_char(out, ' '))
		return (out->error);
	if (out_char(out, 'L'))
		return (out->error);
	if (out_int(out, ant_id))
		return (out->error);
	if (out_char(out, '-'))
		return (out->error);
	if (out_str(out, room->name))
		return (out->error);
	*first = 0;
	return (ERR_NO_ERROR);
}

static int	move_existing_ants(t_print_ctx *ctx, t_path *path, int *lane) {
	t_room	*room;
	int		pos;
	int		ant_id;
	int		*done;
	int		*first;

	done = &ctx->state[1];
	first = &ctx->state[2];
	pos = path->len - 2;
	while (pos >= 0)
	{
		ant_id = lane[pos];
		if (ant_id)
		{
			lane[pos] = 0;
			room = path->rooms.data[pos + 2];
			if (print_move(ctx->out, ant_id, room, first))
				return (ctx->out->error);
			if (pos == path->len - 2)
				(*done)++;
			else
				lane[pos + 1] = ant_id;
		}
		pos--;
	}
	return (ERR_NO_ERROR);
}

static int	send_new_ant(t_print_ctx *ctx, t_path *path, int *lane) {
	t_room	*room;
	int		*next_ant;
	int		*first;

	next_ant = &ctx->state[0];
	first = &ctx->state[2];
	room = path->rooms.data[1];
	if (print_move(ctx->out, *next_ant, room, first))
		return (ctx->out->error);
	if (path->len > 1)
		lane[0] = *next_ant;
	(*next_ant)++;
	return (ERR_NO_ERROR);
}

static int	move_all_existing(t_print_ctx *ctx) {
	t_path	*path;
	int		i;

	i = 0;
	while (i < ctx->solution->path_count)
	{
		path = ctx->solution->paths.data[i];
		if (move_existing_ants(ctx, path, ctx->lanes[i]))
			return (ctx->out->error);
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	send_new_ants(t_print_ctx *ctx) {
	t_path	*path;
	int		i;

	i = 0;
	while (i < ctx->solution->path_count)
	{
		path = ctx->solution->paths.data[i];
		if (path->ants > ctx->state[i + 3])
		{
			if (send_new_ant(ctx, path, ctx->lanes[i]))
				return (ctx->out->error);
			ctx->state[i + 3]++;
			if (path->len == 1)
				ctx->state[1]++;
		}
		i++;
	}
	return (ERR_NO_ERROR);
}

static int	run_one_round(t_print_ctx *ctx) {
	ctx->state[2] = 1;
	if (move_all_existing(ctx))
		return (ctx->out->error);
	if (send_new_ants(ctx))
		return (ctx->out->error);
	return (out_char(ctx->out, '\n'));
}

static int	print_moves(t_outbuf *out, t_farm *farm, t_solution *solution) {
	t_print_ctx	ctx;
	int			err;

	print_ctx_init(&ctx, out, solution);
	err = alloc_print_ctx(&ctx);
	if (err)
		return (err);
	ctx.state[0] = 1;
	ctx.state[1] = 0;
	while (!out->error && ctx.state[1] < farm->ants)
	{
		err = run_one_round(&ctx);
		if (err)
			break ;
	}
	print_ctx_destroy(&ctx);
	return (err);
}

int	print_solution(t_farm *farm, t_solution *solution) {
	t_outbuf	out;
	int			err;

	out.len = 0;
	out.error = ERR_NO_ERROR;
	err = print_farm(&out, farm);
	if (!err)
		err = print_moves(&out, farm, solution);
	if (!err)
		err = out_flush(&out);
	return (err);
}