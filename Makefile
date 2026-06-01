.DEFAULT_GOAL := all

# **************************************************************************** #
#                                  PROGRAMS                                    #
# **************************************************************************** #

NAME		:=	lem-in
BONUS		:=	hex_visu
TESTER		:=	farm_test

# **************************************************************************** #
#                                   COMPILER                                   #
# **************************************************************************** #

CC			:=	cc
CFLAGS		:=	-O3 -Wall -Wextra
DEP_FLAGS	:=	-MMD -MP
RM			:=	rm -rf

SDL_CFLAGS	:=	`sdl2-config --cflags`
SDL_LDFLAGS	:=	`sdl2-config --libs` -lSDL2_gfx -lm

# **************************************************************************** #
#                                  DIRECTORIES                                 #
# **************************************************************************** #

SRCS_DIR	:=	srcs
BONUS_DIR	:=	srcs_bonus
OBJS_DIR	:=	objs
INCLD_DIR	:=	includes
LIBFT_DIR	:=	lib/libft

# **************************************************************************** #
#                                   INCLUDES                                   #
# **************************************************************************** #

INCLD_FLAG	:=	-I $(INCLD_DIR)

# **************************************************************************** #
#                                    LIBFT                                     #
# **************************************************************************** #

LIBFT		:=	$(LIBFT_DIR)/libft.a

# **************************************************************************** #
#                                    COLORS                                    #
# **************************************************************************** #

NEW			:=	\r\033[K
DEFAULT		:=	\033[0m
RED			:=	\033[0;31m
GREEN		:=	\033[0;32m
U_GREEN		:=	\033[4;32m
NEON_GREEN	:=	\033[38;5;82m
YELLOW		:=	\033[0;33m
CYAN		:=	\033[0;36m

# **************************************************************************** #
#                                    SOURCES                                   #
# **************************************************************************** #

SRCS_CORE	:=	\
				srcs/error.c \
				srcs/utils.c \
				srcs/farm/farm.c \
				srcs/parsing/parse_utils.c \
				srcs/parsing/parse_ants.c \
				srcs/parsing/parse_rooms.c \
				srcs/parsing/parse_links.c 


SRCS_MANDATORY	:=	\
				srcs/main.c \
				srcs/parsing/parse_farm.c \
				srcs/solver/solver.c \
				srcs/solver/solution.c \
				srcs/solver/flow_graph.c \
				srcs/solver/flow_edges.c \
				srcs/solver/min_cost_flow.c \
				srcs/solver/extract_paths.c \
				srcs/solver/choose_solution.c \
				srcs/solver/print_solution.c

SRCS_BONUS	:=	\
				srcs_bonus/main_visualizer.c \
				srcs_bonus/parsing/parse_visu_input.c \
				srcs_bonus/parsing/parse_move_line.c \
				srcs_bonus/visualizer/sdl.c \
				srcs_bonus/visualizer/camera.c \
				srcs_bonus/visualizer/events.c \
				srcs_bonus/visualizer/timeline.c \
				srcs_bonus/visualizer/draw_scene.c \
				srcs_bonus/visualizer/draw_ants.c \
				srcs_bonus/visualizer/visualizer_destroy.c \
				srcs_bonus/visualizer/metrics.c \
				srcs_bonus/visualizer/colors.c \
				srcs_bonus/visualizer/animation.c \
				srcs_bonus/visualizer/background.c

SRCS_TESTER	:=	\
				srcs/debug/main_farm_test.c \
				srcs/debug/dump_farm.c \
				srcs/parsing/parse_farm.c

# **************************************************************************** #
#                                    OBJECTS                                   #
# **************************************************************************** #

OBJS_CORE		:=	$(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS_CORE))
OBJS_MANDATORY	:=	$(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS_MANDATORY))
OBJS_BONUS		:=	$(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS_BONUS))
OBJS_TESTER		:=	$(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS_TESTER))

DEPS			:=	\
					$(OBJS_CORE:.o=.d) \
					$(OBJS_MANDATORY:.o=.d) \
					$(OBJS_BONUS:.o=.d) \
					$(OBJS_TESTER:.o=.d)

# **************************************************************************** #
#                                  COMPILATION                                 #
# **************************************************************************** #

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(OBJS_BONUS): CFLAGS_LOCAL := $(SDL_CFLAGS)

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "  $(YELLOW)🔨 Compiling $(DEFAULT)%-50s\r" "$<"
	@$(CC) $(DEP_FLAGS) $(CFLAGS) $(CFLAGS_LOCAL) $(INCLD_FLAG) -c $< -o $@ 2>&1 | sed 's/^/    /' || (echo ""; exit 1)
	@printf "  $(GREEN)✓ Compiled  $(DEFAULT)%-50s\n" "$<"

# **************************************************************************** #
#                                     RULES                                    #
# **************************************************************************** #

.PHONY: banner

banner:
	@printf "\n\033[32m"
	@printf "╔═════════════════════════════════════════════════════════════════════╗\n"
	@printf "║                                                                     ║\n"
	@printf "║           ██╗     ███████╗███╗   ███╗    ██╗███╗   ██╗              ║\n"
	@printf "║           ██║     ██╔════╝████╗ ████║    ██║████╗  ██║              ║\n"
	@printf "║           ██║     █████╗  ██╔████╔██║    ██║██╔██╗ ██║              ║\n"
	@printf "║           ██║     ██╔══╝  ██║╚██╔╝██║    ██║██║╚██╗██║              ║\n"
	@printf "║           ███████╗███████╗██║ ╚═╝ ██║    ██║██║ ╚████║              ║\n"
	@printf "║           ╚══════╝╚══════╝╚═╝     ╚═╝    ╚═╝╚═╝  ╚═══╝              ║\n"
	@printf "║                                                                     ║\n"
	@printf "║                     \\_/     \\_/     \\_/                             ║\n"
	@printf "║                    (o.o)   (o.o)   (o.o)                            ║\n"
	@printf "║                    /> <\\   /> <\\   /> <\\                            ║\n"
	@printf "║                                                                     ║\n"
	@printf "║                   Ant Colony Pathfinding Solver                     ║\n"
	@printf "║                                                                     ║\n"
	@printf "╚═════════════════════════════════════════════════════════════════════╝\033[0m\n"
	@printf "\n"

all: banner $(NAME) $(BONUS) $(TESTER)

$(NAME): $(LIBFT) $(OBJS_CORE) $(OBJS_MANDATORY)
	@printf "\n  $(YELLOW)⏳ Linking $(NAME)...$(DEFAULT) "
	@for i in $$(seq 1 30); do printf "$(GREEN)█$(DEFAULT)"; sleep 0.01; done
	@printf "\n"
	@$(CC) $(OBJS_CORE) $(OBJS_MANDATORY) $(CFLAGS) $(LIBFT) -lm -o $(NAME)
	@printf "  $(GREEN)✅ Binary compiled: ./$(NAME)$(DEFAULT)\n\n"

$(BONUS): $(LIBFT) $(OBJS_CORE) $(OBJS_BONUS)
	@printf "\n  $(YELLOW)⏳ Linking $(BONUS)...$(DEFAULT) "
	@for i in $$(seq 1 30); do printf "$(GREEN)█$(DEFAULT)"; sleep 0.01; done
	@printf "\n"
	@$(CC) $(OBJS_CORE) $(OBJS_BONUS) $(CFLAGS) $(LIBFT) $(SDL_LDFLAGS) -o $(BONUS) 2>/dev/null || echo "  $(YELLOW)⚠ SDL not available$(DEFAULT)"
	@printf "  $(GREEN)✅ Bonus compiled (if no errors)$(DEFAULT)\n\n"

$(TESTER): $(LIBFT) $(OBJS_CORE) $(OBJS_TESTER)
	@printf "\n  $(YELLOW)⏳ Linking $(TESTER)...$(DEFAULT) "
	@for i in $$(seq 1 30); do printf "$(GREEN)█$(DEFAULT)"; sleep 0.01; done
	@printf "\n"
	@$(CC) $(OBJS_CORE) $(OBJS_TESTER) $(CFLAGS) $(LIBFT) -lm -o $(TESTER)
	@printf "  $(GREEN)✅ Tester compiled: ./$(TESTER)$(DEFAULT)\n\n"

bonus: $(BONUS)

tester: $(TESTER)

MAP			?=	test/map1000

test: $(TESTER)
	@./$(TESTER) < $(MAP)

# **************************************************************************** #
#                                   CLEANING                                   #
# **************************************************************************** #

clean:
	@printf "[$(CYAN)lem-in$(DEFAULT)] $(RED)Cleaning objects$(DEFAULT)\n"
	@$(RM) $(OBJS_DIR)
	@make clean -sC $(LIBFT_DIR) > /dev/null 2>&1
	@printf "[$(CYAN)lem-in$(DEFAULT)] $(GREEN)Objects cleaned$(DEFAULT)\n"

fclean: clean
	@printf "[$(CYAN)lem-in$(DEFAULT)] $(RED)Deleting executables$(DEFAULT)\n"
	@$(RM) $(NAME) $(BONUS) $(TESTER) $(LIBFT)
	@make fclean -sC $(LIBFT_DIR) > /dev/null 2>&1
	@printf "[$(CYAN)lem-in$(DEFAULT)] $(NEON_GREEN)Done!$(DEFAULT)\n"

re: fclean all

test_invalid: $(TESTER)
	@printf "$(CYAN)Testing all invalid maps...$(DEFAULT)\n"
	@passed=0; failed=0; total=0; \
	for map in maps/invalid/*.map; do \
		total=$$((total + 1)); \
		if ./$(TESTER) < "$$map" > /dev/null 2>&1; then \
			printf "$(RED)✗ FAIL: %s$(DEFAULT) (should have failed)\n" "$$(basename $$map)"; \
			failed=$$((failed + 1)); \
		else \
			printf "$(GREEN)✓ PASS: %s$(DEFAULT)\n" "$$(basename $$map)"; \
			passed=$$((passed + 1)); \
		fi; \
	done; \
	printf "\n"; \
	printf "$(CYAN)Results: $(GREEN)%d passed$(DEFAULT), $(RED)%d failed$(DEFAULT), %d total$(DEFAULT)\n" $$passed $$failed $$total; \
	[ $$failed -eq 0 ]

-include $(DEPS)

.PHONY: all bonus tester test clean fclean re test_invalid banner