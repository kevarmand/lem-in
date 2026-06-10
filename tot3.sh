#!/bin/sh
set -eu

MAP="${1:-fractal/fractal04.map}"

make -C lib/libft

cc -O3 -Wall -Wextra -I includes \
	srcs/debug/profile_main.c \
	srcs/error.c \
	srcs/utils.c \
	srcs/farm/farm.c \
	srcs/parsing/parse_utils.c \
	srcs/parsing/parse_ants.c \
	srcs/parsing/parse_rooms.c \
	srcs/parsing/parse_links.c \
	srcs/parsing/parse_farm.c \
	srcs/solver/solver.c \
	srcs/solver/solution.c \
	srcs/solver/flow_graph.c \
	srcs/solver/flow_edges.c \
	srcs/solver/min_cost_flow.c \
	srcs/solver/extract_paths.c \
	srcs/solver/choose_solution.c \
	srcs/solver/print_solution.c \
	lib/libft/libft.a \
	-lm \
	-o lem_profile

./lem_profile < "$MAP" > /dev/null