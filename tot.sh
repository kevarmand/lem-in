#!/bin/sh

OUT=dump.txt
: > "$OUT"

add()
{
	if [ -f "$1" ]; then
		printf '\n\n===== %s =====\n\n' "$1" >> "$OUT"
		cat "$1" >> "$OUT"
	fi
}

add Makefile
add includes/lem_in.h
add includes/parser.h
add includes/solver.h
add includes/visualizer.h

find srcs -type f \( -name "*.c" -o -name "*.h" \) | sort | while read f; do
	case "$f" in
		*parser*|*parse*|*map*|*room*|*link*|*cleanup*|*free*|*main*|*error*|*reader*|*gnl*)
			add "$f"
			;;
	esac
done