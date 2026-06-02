#!/bin/sh

OUT="dump_leak.txt"

: > "$OUT"

section()
{
	printf "\n\n==================== %s ====================\n\n" "$1" >> "$OUT"
}

dump_cmd()
{
	section "$1"
	shift
	"$@" >> "$OUT" 2>&1
}

dump_file()
{
	if [ -f "$1" ]; then
		section "$1"
		nl -ba "$1" >> "$OUT"
	fi
}

section "CONTEXT"
cat >> "$OUT" <<'EOF'
Current bug:
- solver disabled
- print disabled
- farm_destroy called
- valgrind still reports leaks
- likely area: farm_destroy / room_destroy / link_destroy / vector_destroy / hashmap_destroy
EOF

dump_cmd "GIT STATUS" git status --short
dump_cmd "CURRENT BRANCH" git branch --show-current
dump_cmd "MAKE TIMESTAMPS" ls -la lem-in lib/libft/libft.a
dump_cmd "OBJECT TIMESTAMPS CORE" find objs/srcs lib/libft -type f \( -name "*.o" -o -name "*.a" \) -printf "%TY-%Tm-%Td %TH:%TM:%TS %p\n" | sort

dump_file Makefile

section "CURRENT MAIN / FARM / PARSING"
dump_file srcs/main.c
dump_file srcs/farm/farm.c
dump_file srcs/parsing/parse_farm.c
dump_file srcs/parsing/parse_ants.c
dump_file srcs/parsing/parse_rooms.c
dump_file srcs/parsing/parse_links.c
dump_file srcs/parsing/parse_utils.c
dump_file srcs/utils.c

section "CURRENT HEADERS"
dump_file includes/lem_in.h
dump_file includes/farm.h
dump_file includes/parsing.h
dump_file includes/utils.h
dump_file includes/error.h
dump_file includes/libft.h

section "LIBFT VECTOR / HASHMAP FULL"
dump_file lib/libft/libft.h
dump_file lib/libft/vector.c
dump_file lib/libft/hashmap/hashmap.h
dump_file lib/libft/hashmap/hashmap_memory.c
dump_file lib/libft/hashmap/hashmap_insert.c
dump_file lib/libft/hashmap/hashmap_get.c
dump_file lib/libft/hashmap/hashmap_rehash.c
dump_file lib/libft/hashmap/hashmap_remove.c
dump_file lib/libft/hashmap/hashmap_hashf.c

section "LIBFT ALLOC HELPERS FULL"
dump_file lib/libft/ft_calloc.c
dump_file lib/libft/ft_strdup.c
dump_file lib/libft/ft_strjoin.c
dump_file lib/libft/ft_itoa.c
dump_file lib/libft/ft_strlen.c
dump_file lib/libft/ft_strcmp.c
dump_file lib/libft/ft_strchr.c
dump_file lib/libft/ft_strncmp.c

section "VECTOR DESTROY / INIT / PUSH CALLS"
grep -RIn \
	-e "vector_init" \
	-e "vector_destroy" \
	-e "vector_push_back" \
	-e "room_destroy" \
	-e "link_destroy" \
	-e "farm_destroy" \
	-e "destroy_vector_content" \
	srcs includes lib/libft >> "$OUT" 2>&1

section "HASHMAP DESTROY / INSERT CALLS"
grep -RIn \
	-e "hashmap_create" \
	-e "hashmap_insert" \
	-e "hashmap_destroy" \
	-e "hashmap_rehash" \
	-e "hashmap_remove" \
	srcs includes lib/libft >> "$OUT" 2>&1

section "ALLOC / FREE IN PARSING FARM VECTOR HASHMAP"
grep -RIn \
	-e "malloc" \
	-e "free" \
	-e "ft_calloc" \
	-e "ft_strdup" \
	-e "ft_strjoin" \
	-e "ft_itoa" \
	-e "get_next_line" \
	srcs/farm srcs/parsing srcs/utils.c lib/libft/vector.c lib/libft/hashmap lib/libft/ft_calloc.c lib/libft/ft_strdup.c lib/libft/ft_strjoin.c lib/libft/ft_itoa.c >> "$OUT" 2>&1

section "SYMBOLS IN LEM-IN"
nm -C ./lem-in | grep -E "vector_destroy|vector_init|vector_push_back|farm_destroy|room_destroy|link_destroy|hashmap_destroy" >> "$OUT" 2>&1

section "VALGRIND FULL CURRENT"
valgrind --leak-check=full ./lem-in < toto > /dev/null 2>> "$OUT"

printf "dump generated in %s\n" "$OUT"