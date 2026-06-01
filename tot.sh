#!/bin/sh

OUT="dump.txt"
: > "$OUT"

dump_file() {
	if [ -f "$1" ]; then
		printf "\n\n===== %s =====\n" "$1" >> "$OUT"
		cat "$1" >> "$OUT"
	fi
}

dump_dir() {
	if [ -d "$1" ]; then
		find "$1" -type f \( -name "*.h" -o -name "*.c" \) | sort | while read file; do
			dump_file "$file"
		done
	fi
}

dump_file Makefile
dump_dir includes
dump_dir srcs_bonus

printf "dump written in %s\n" "$OUT"