#include <stdlib.h>
#include "visualizer.h"
#include "libft.h"
#include "error.h"

#define REORG_ORDER_PASSES 6

typedef struct s_order_ctx
{
	t_farm	*farm;
	int		*dist;
	int		*used;
	int		*order;
	int		*ids;
	int		count;
}	t_order_ctx;

static int	order_init(t_order_ctx *ctx, t_farm *farm, int *dist, int *used);
static void	order_destroy(t_order_ctx *ctx);
static int	order_max_dist(t_order_ctx *ctx);
static void	order_compact_all(t_order_ctx *ctx, int max_dist);
static void	order_compact_layer(t_order_ctx *ctx, int distance);
static int	order_collect_layer(t_order_ctx *ctx, int distance);
static void	order_collect_matching(t_order_ctx *ctx, int distance, int used,
				int *count);
static void	order_assign_layer(t_order_ctx *ctx, int count);
static void	order_forward_pass(t_order_ctx *ctx, int max_dist);
static void	order_backward_pass(t_order_ctx *ctx, int max_dist);
static void	order_sort_layer(t_order_ctx *ctx, int distance, int target);
static void	order_insertion_sort(t_order_ctx *ctx, int count, int target);
static int	order_after(t_order_ctx *ctx, int left, int right, int target);
static double	order_score(t_order_ctx *ctx, int room_id, int target);
static int	order_edge_weight(t_order_ctx *ctx, int a, int b);

int	visu_reorg_build_order(t_farm *farm, int *dist, int *used, int *order) {
	t_order_ctx	ctx;
	int			max_dist;
	int			pass;

	if (order_init(&ctx, farm, dist, used))
		return (ERR_MALLOC);
	ctx.order = order;
	max_dist = order_max_dist(&ctx);
	order_compact_all(&ctx, max_dist);
	pass = 0;
	while (pass < REORG_ORDER_PASSES)
	{
		order_forward_pass(&ctx, max_dist);
		order_backward_pass(&ctx, max_dist);
		pass++;
	}
	order_destroy(&ctx);
	return (ERR_NO_ERROR);
}

static int	order_init(t_order_ctx *ctx, t_farm *farm, int *dist, int *used) {
	ctx->farm = farm;
	ctx->dist = dist;
	ctx->used = used;
	ctx->order = NULL;
	ctx->count = (int)farm->rooms.count;
	ctx->ids = malloc(sizeof(*ctx->ids) * ctx->count);
	if (!ctx->ids)
		return (ERR_MALLOC);
	return (ERR_NO_ERROR);
}

static void	order_destroy(t_order_ctx *ctx) {
	free(ctx->ids);
	ctx->ids = NULL;
}

static int	order_max_dist(t_order_ctx *ctx) {
	int	max;
	int	i;

	max = 0;
	i = 0;
	while (i < ctx->count)
	{
		if (ctx->dist[i] > max)
			max = ctx->dist[i];
		i++;
	}
	return (max);
}

static void	order_compact_all(t_order_ctx *ctx, int max_dist) {
	int	distance;

	order_compact_layer(ctx, -1);
	distance = 0;
	while (distance <= max_dist)
	{
		order_compact_layer(ctx, distance);
		distance++;
	}
}

static void	order_compact_layer(t_order_ctx *ctx, int distance) {
	int	count;

	count = 0;
	order_collect_matching(ctx, distance, 1, &count);
	order_collect_matching(ctx, distance, 0, &count);
	order_assign_layer(ctx, count);
}

static int	order_collect_layer(t_order_ctx *ctx, int distance) {
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < ctx->count)
	{
		if (ctx->dist[i] == distance)
		{
			ctx->ids[count] = i;
			count++;
		}
		i++;
	}
	return (count);
}

static void	order_collect_matching(t_order_ctx *ctx, int distance, int used,
	int *count) {
	int	i;

	i = 0;
	while (i < ctx->count)
	{
		if (ctx->dist[i] == distance && ctx->used[i] == used)
		{
			ctx->ids[*count] = i;
			(*count)++;
		}
		i++;
	}
}

static void	order_assign_layer(t_order_ctx *ctx, int count) {
	int	i;

	i = 0;
	while (i < count)
	{
		ctx->order[ctx->ids[i]] = i;
		i++;
	}
}

static void	order_forward_pass(t_order_ctx *ctx, int max_dist) {
	int	distance;

	distance = 1;
	while (distance <= max_dist)
	{
		order_sort_layer(ctx, distance, distance - 1);
		distance++;
	}
}

static void	order_backward_pass(t_order_ctx *ctx, int max_dist) {
	int	distance;

	distance = max_dist - 1;
	while (distance >= 1)
	{
		order_sort_layer(ctx, distance, distance + 1);
		distance--;
	}
}

static void	order_sort_layer(t_order_ctx *ctx, int distance, int target) {
	int	count;

	count = order_collect_layer(ctx, distance);
	if (count <= 1)
		return ;
	order_insertion_sort(ctx, count, target);
	order_assign_layer(ctx, count);
}

static void	order_insertion_sort(t_order_ctx *ctx, int count, int target) {
	int	i;
	int	j;
	int	key;

	i = 1;
	while (i < count)
	{
		key = ctx->ids[i];
		j = i - 1;
		while (j >= 0 && order_after(ctx, ctx->ids[j], key, target))
		{
			ctx->ids[j + 1] = ctx->ids[j];
			j--;
		}
		ctx->ids[j + 1] = key;
		i++;
	}
}

static int	order_after(t_order_ctx *ctx, int left, int right, int target) {
	double	left_score;
	double	right_score;

	if (ctx->used[left] != ctx->used[right])
		return (ctx->used[left] < ctx->used[right]);
	left_score = order_score(ctx, left, target);
	right_score = order_score(ctx, right, target);
	if (left_score > right_score)
		return (1);
	if (left_score < right_score)
		return (0);
	return (ctx->order[left] > ctx->order[right]);
}

static double	order_score(t_order_ctx *ctx, int room_id, int target) {
	t_room	*room;
	t_room	*neighbor;
	double	sum;
	double	weight_sum;
	int		weight;
	size_t	i;

	room = ctx->farm->rooms.data[room_id];
	sum = 0.0;
	weight_sum = 0.0;
	i = 0;
	while (i < room->neighbors.count)
	{
		neighbor = room->neighbors.data[i];
		if (ctx->dist[neighbor->id] == target)
		{
			weight = order_edge_weight(ctx, room_id, neighbor->id);
			sum += (double)ctx->order[neighbor->id] * (double)weight;
			weight_sum += (double)weight;
		}
		i++;
	}
	if (weight_sum == 0.0)
		return ((double)ctx->order[room_id]);
	return (sum / weight_sum);
}

static int	order_edge_weight(t_order_ctx *ctx, int a, int b) {
	if (ctx->used[a] && ctx->used[b])
		return (8);
	return (1);
}