#!/bin/sh

OUT="dump.txt"

: > "$OUT"

dump_file()
{
	if [ ! -f "$1" ]; then
		return
	fi
	printf "\n\n==================== %s ====================\n\n" "$1" >> "$OUT"
	cat "$1" >> "$OUT"
}

dump_file "includes/visualizer.h"

find srcs_bonus -type f | sort | while IFS= read -r file
do
	dump_file "$file"
done

printf "Dump written to %s\n" "$OUT"