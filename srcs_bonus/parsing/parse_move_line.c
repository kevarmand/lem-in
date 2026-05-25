#include <stdlib.h>
#include "visualizer.h"
#include "utils.h"
#include <stdlib.h>
#include "visualizer.h"
#include "utils.h"

static int	parse_step(t_farm *farm, t_visu *visu, char *line);
static int	parse_move_token(t_farm *farm, t_visu *visu, t_step *step, char *word);
static int	room_has_neighbor(t_room *from, t_room *to);
static void	step_destroy(void *ptr);

int	parse_move_line(int *err, char **line, t_farm *farm, t_visu *visu) {
	if (*err || !*line)
		return (0);
	parse_strip_newline(*line);
	if ((*line)[0] == '\0')
	{
		parse_consume_line(line);
		return (1);
	}
	if ((*line)[0] == '#')
	{
		parse_consume_line(line);
		return (1);
	}
	if ((*line)[0] != 'L')
		return (0);
	*err = parse_step(farm, visu, *line);
	parse_consume_line(line);
	return (1);
}

static int	parse_step(t_farm *farm, t_visu *visu, char *line) {
	t_step	*step;
	char	*word;
	char	*next;
	int		err;

	step = malloc(sizeof(*step));
	if (!step)
		return (ERR_MALLOC);
	if (vector_init(&step->moves, 8))
	{
		free(step);
		return (ERR_MALLOC);
	}
	err = ERR_NO_ERROR;
	word = line;
	while (!err && *word)
	{
		while (*word == ' ')
			word++;
		if (!*word)
			break ;
		next = ft_strchr(word, ' ');
		if (next)
			*next = '\0';
		err = parse_move_token(farm, visu, step, word);
		if (!next)
			break ;
		word = next + 1;
	}
	if (!err && vector_push_back(&visu->anim.steps, step))
		err = ERR_MALLOC;
	if (err)
		step_destroy(step);
	else
		visu->anim.step_count = (int)visu->anim.steps.count;
	return (err);
}

static int	parse_move_token(t_farm *farm, t_visu *visu, t_step *step, char *word) {
	t_move	*move;
	t_room	*to;
	char	*dash;
	int		id;

	if (word[0] != 'L')
		return (ERR_MOVE);
	dash = ft_strchr(word, '-');
	if (!dash)
		return (ERR_MOVE);
	*dash = '\0';
	if (custom_atoi(word + 1, &id) || id <= 0 || id > farm->ants)
		return (ERR_MOVE);
	to = hashmap_get(farm->rooms_by_name, dash + 1);
	if (!to)
		return (ERR_MOVE);
	if (visu->anim.ants[id].arrived)
		return (ERR_MOVE);
	if (!room_has_neighbor(visu->anim.ants[id].room, to))
		return (ERR_MOVE);
	move = malloc(sizeof(*move));
	if (!move)
		return (ERR_MALLOC);
	move->ant_id = id;
	move->from = visu->anim.ants[id].room;
	move->to = to;
	move->path_id = visu->anim.ants[id].path_id;
	if (vector_push_back(&step->moves, move))
	{
		free(move);
		return (ERR_MALLOC);
	}
	visu->anim.ants[id].room = to;
	if (to == farm->end)
		visu->anim.ants[id].arrived = 1;
	return (ERR_NO_ERROR);
}

static int	room_has_neighbor(t_room *from, t_room *to) {
	size_t	i;

	i = 0;
	while (i < from->neighbors.count)
	{
		if (from->neighbors.data[i] == to)
			return (1);
		i++;
	}
	return (0);
}

static void	step_destroy(void *ptr) {
	t_step	*step;
	size_t	i;

	step = (t_step *)ptr;
	if (!step)
		return ;
	i = 0;
	while (i < step->moves.count)
	{
		free(step->moves.data[i]);
		i++;
	}
	vector_destroy(&step->moves);
	free(step);
}