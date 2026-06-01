#include <stdlib.h>
#include <limits.h>
#include "visualizer.h"
#include "libft.h"
#include "error.h"

typedef struct s_reorg
{
	t_farm	*farm;
	int		*dist;
	int		*parent;
	int		*queue;
	int		*used;
	int		*order;
	int		*layer_ids;
	int		queue_count;
	int		room_count;
	int		max_dist;
	int		diag;
}	t_reorg;

typedef struct s_slot
{
	int	x;
	int	y;
	int	used;
}	t_slot;

typedef struct s_block
{
	t_slot	*slots;
	int		count;
}	t_block;

static int	reorg_init(t_reorg *reorg, t_visu *visu);
static void	reorg_destroy(t_reorg *reorg);
static void	reorg_mark_used(t_reorg *reorg, t_visu *visu);
static void	reorg_bfs(t_reorg *reorg);
static void	reorg_push_neighbors(t_reorg *reorg, t_room *room, int *tail);
static int	reorg_place(t_reorg *reorg);
static int	reorg_place_layer(t_reorg *reorg, int distance);
static int	reorg_place_unreachable(t_reorg *reorg);
static void	reorg_place_layer_rooms(t_reorg *reorg, t_block *block,
				int distance);
static int	reorg_collect_layer(t_reorg *reorg, int distance);
static void	reorg_sort_layer(t_reorg *reorg, int count);
static int	reorg_after(t_reorg *reorg, int left, int right);
static void	reorg_place_room(t_reorg *reorg, t_block *block, t_room *room);
static int	reorg_count_layer(t_reorg *reorg, int distance);
static int	reorg_reserve_block(int begin, int room_count);
static int	block_init(t_block *block, int begin, int end);
static void	block_fill(t_block *block, int begin, int end);
static int	block_capacity(int begin, int end);
static int	block_take_slot(t_block *block, t_room *parent);
static int	slot_score(t_slot *slot, t_room *parent);
static int	abs_int(int value);

int	visu_reorganize_rooms(t_visu *visu) {
	t_reorg	reorg;
	int		err;

	err = reorg_init(&reorg, visu);
	if (err)
		return (err);
	reorg_bfs(&reorg);
	err = visu_reorg_build_order(reorg.farm, reorg.dist, reorg.used,
			reorg.order);
	if (!err)
		err = reorg_place(&reorg);
	reorg_destroy(&reorg);
	if (!err)
		visu->layout.mode = LAYOUT_PATHS;
	return (err);
}

static int	reorg_init(t_reorg *reorg, t_visu *visu) {
	int	i;

	reorg->farm = visu->farm;
	reorg->room_count = (int)visu->farm->rooms.count;
	reorg->queue_count = 0;
	reorg->max_dist = 0;
	reorg->diag = 2;
	reorg->dist = malloc(sizeof(*reorg->dist) * reorg->room_count);
	reorg->parent = malloc(sizeof(*reorg->parent) * reorg->room_count);
	reorg->queue = malloc(sizeof(*reorg->queue) * reorg->room_count);
	reorg->used = ft_calloc(reorg->room_count, sizeof(*reorg->used));
	reorg->order = malloc(sizeof(*reorg->order) * reorg->room_count);
	reorg->layer_ids = malloc(sizeof(*reorg->layer_ids) * reorg->room_count);
	if (!reorg->dist || !reorg->parent || !reorg->queue || !reorg->used
		|| !reorg->order || !reorg->layer_ids)
	{
		reorg_destroy(reorg);
		return (ERR_MALLOC);
	}
	i = 0;
	while (i < reorg->room_count)
	{
		reorg->dist[i] = -1;
		reorg->parent[i] = -1;
		reorg->order[i] = i;
		i++;
	}
	reorg_mark_used(reorg, visu);
	return (ERR_NO_ERROR);
}

static void	reorg_destroy(t_reorg *reorg) {
	free(reorg->dist);
	free(reorg->parent);
	free(reorg->queue);
	free(reorg->used);
	free(reorg->order);
	free(reorg->layer_ids);
	reorg->dist = NULL;
	reorg->parent = NULL;
	reorg->queue = NULL;
	reorg->used = NULL;
	reorg->order = NULL;
	reorg->layer_ids = NULL;
}

static void	reorg_mark_used(t_reorg *reorg, t_visu *visu) {
	t_step	*step;
	t_move	*move;
	size_t	i;
	size_t	j;

	reorg->used[visu->farm->start->id] = 1;
	reorg->used[visu->farm->end->id] = 1;
	i = 0;
	while (i < visu->anim.steps.count)
	{
		step = (t_step *)visu->anim.steps.data[i];
		j = 0;
		while (j < step->moves.count)
		{
			move = (t_move *)step->moves.data[j];
			reorg->used[move->from->id] = 1;
			reorg->used[move->to->id] = 1;
			j++;
		}
		i++;
	}
}

static void	reorg_bfs(t_reorg *reorg) {
	t_room	*room;
	int		head;
	int		tail;

	head = 0;
	tail = 0;
	reorg->dist[reorg->farm->start->id] = 0;
	reorg->queue[tail++] = reorg->farm->start->id;
	while (head < tail)
	{
		room = reorg->farm->rooms.data[reorg->queue[head++]];
		if (reorg->dist[room->id] > reorg->max_dist)
			reorg->max_dist = reorg->dist[room->id];
		reorg_push_neighbors(reorg, room, &tail);
	}
	reorg->queue_count = tail;
}

static void	reorg_push_neighbors(t_reorg *reorg, t_room *room, int *tail) {
	t_room	*next;
	size_t	i;

	i = 0;
	while (i < room->neighbors.count)
	{
		next = room->neighbors.data[i];
		if (reorg->dist[next->id] == -1)
		{
			reorg->dist[next->id] = reorg->dist[room->id] + 1;
			reorg->parent[next->id] = room->id;
			reorg->queue[*tail] = next->id;
			(*tail)++;
		}
		i++;
	}
}

static int	reorg_place(t_reorg *reorg) {
	int	distance;
	int	err;

	reorg->farm->start->x = 0;
	reorg->farm->start->y = 0;
	distance = 1;
	while (distance <= reorg->max_dist)
	{
		err = reorg_place_layer(reorg, distance);
		if (err)
			return (err);
		distance++;
	}
	return (reorg_place_unreachable(reorg));
}

static int	reorg_place_layer(t_reorg *reorg, int distance) {
	t_block	block;
	int		count;
	int		end;
	int		err;

	count = reorg_count_layer(reorg, distance);
	if (!count)
		return (ERR_NO_ERROR);
	end = reorg_reserve_block(reorg->diag, count);
	err = block_init(&block, reorg->diag, end);
	if (err)
		return (err);
	reorg_place_layer_rooms(reorg, &block, distance);
	free(block.slots);
	reorg->diag = end + 2;
	return (ERR_NO_ERROR);
}

static int	reorg_place_unreachable(t_reorg *reorg) {
	t_block	block;
	int		count;
	int		end;
	int		err;

	count = reorg_count_layer(reorg, -1);
	if (!count)
		return (ERR_NO_ERROR);
	end = reorg_reserve_block(reorg->diag, count);
	err = block_init(&block, reorg->diag, end);
	if (err)
		return (err);
	reorg_place_layer_rooms(reorg, &block, -1);
	free(block.slots);
	reorg->diag = end + 2;
	return (ERR_NO_ERROR);
}

static void	reorg_place_layer_rooms(t_reorg *reorg, t_block *block,
	int distance) {
	t_room	*room;
	int		count;
	int		i;

	count = reorg_collect_layer(reorg, distance);
	reorg_sort_layer(reorg, count);
	i = 0;
	while (i < count)
	{
		room = reorg->farm->rooms.data[reorg->layer_ids[i]];
		reorg_place_room(reorg, block, room);
		i++;
	}
}

static int	reorg_collect_layer(t_reorg *reorg, int distance) {
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < reorg->room_count)
	{
		if (reorg->dist[i] == distance)
		{
			reorg->layer_ids[count] = i;
			count++;
		}
		i++;
	}
	return (count);
}

static void	reorg_sort_layer(t_reorg *reorg, int count) {
	int	i;
	int	j;
	int	key;

	i = 1;
	while (i < count)
	{
		key = reorg->layer_ids[i];
		j = i - 1;
		while (j >= 0 && reorg_after(reorg, reorg->layer_ids[j], key))
		{
			reorg->layer_ids[j + 1] = reorg->layer_ids[j];
			j--;
		}
		reorg->layer_ids[j + 1] = key;
		i++;
	}
}

static int	reorg_after(t_reorg *reorg, int left, int right) {
	if (reorg->used[left] != reorg->used[right])
		return (reorg->used[left] < reorg->used[right]);
	if (reorg->order[left] != reorg->order[right])
		return (reorg->order[left] > reorg->order[right]);
	return (left > right);
}

static void	reorg_place_room(t_reorg *reorg, t_block *block, t_room *room) {
	t_room	*parent;
	int		slot;
	int		parent_id;

	parent = NULL;
	parent_id = reorg->parent[room->id];
	if (parent_id >= 0)
		parent = reorg->farm->rooms.data[parent_id];
	slot = block_take_slot(block, parent);
	room->x = block->slots[slot].x;
	room->y = block->slots[slot].y;
	block->slots[slot].used = 1;
}

static int	reorg_count_layer(t_reorg *reorg, int distance) {
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < reorg->room_count)
	{
		if (reorg->dist[i] == distance)
			count++;
		i++;
	}
	return (count);
}

static int	reorg_reserve_block(int begin, int room_count) {
	int	end;
	int	capacity;

	end = begin;
	capacity = begin + 1;
	while (capacity < room_count)
	{
		end++;
		capacity += end + 1;
	}
	return (end);
}

static int	block_init(t_block *block, int begin, int end) {
	block->count = block_capacity(begin, end);
	block->slots = malloc(sizeof(*block->slots) * block->count);
	if (!block->slots)
		return (ERR_MALLOC);
	block_fill(block, begin, end);
	return (ERR_NO_ERROR);
}

static void	block_fill(t_block *block, int begin, int end) {
	int	diag;
	int	x;
	int	i;

	i = 0;
	diag = begin;
	while (diag <= end)
	{
		x = 0;
		while (x <= diag)
		{
			block->slots[i].x = x;
			block->slots[i].y = diag - x;
			block->slots[i].used = 0;
			i++;
			x++;
		}
		diag++;
	}
}

static int	block_capacity(int begin, int end) {
	int	capacity;
	int	diag;

	capacity = 0;
	diag = begin;
	while (diag <= end)
	{
		capacity += diag + 1;
		diag++;
	}
	return (capacity);
}

static int	block_take_slot(t_block *block, t_room *parent) {
	int	i;
	int	best;
	int	score;
	int	best_score;

	i = 0;
	best = 0;
	best_score = INT_MAX;
	while (i < block->count)
	{
		if (!block->slots[i].used)
		{
			score = slot_score(&block->slots[i], parent);
			if (score < best_score)
			{
				best = i;
				best_score = score;
			}
		}
		i++;
	}
	return (best);
}

static int	slot_score(t_slot *slot, t_room *parent) {
	int	score;

	score = abs_int(slot->x - slot->y);
	if (parent)
		score += (abs_int(slot->x - parent->x)
				+ abs_int(slot->y - parent->y)) * 1000;
	return (score);
}

static int	abs_int(int value) {
	if (value < 0)
		return (-value);
	return (value);
}