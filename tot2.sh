#!/bin/sh

OUT="dump_mandatory.txt"

rm -f "$OUT"

{
	echo "===== GIT STATUS ====="
	git status --short 2>/dev/null || true
	echo

	echo "===== TREE ====="
	tree srcs includes lib/libft 2>/dev/null || find srcs includes lib/libft -print
	echo

	echo "===== MAKEFILE ====="
	if [ -f Makefile ]; then
		echo "----- FILE: Makefile -----"
		cat Makefile
		echo
	fi

	echo "===== HEADERS ====="
	find includes lib/libft -type f \( -name '*.h' -o -name '*.hpp' \) | sort | while read -r file
	do
		echo "----- FILE: $file -----"
		cat "$file"
		echo
	done

	echo "===== SRC MANDATORY ====="
	find srcs -type f \( -name '*.c' -o -name '*.h' \) | sort | while read -r file
	do
		echo "----- FILE: $file -----"
		cat "$file"
		echo
	done

	echo "===== LIBFT CORE ====="
	find lib/libft -maxdepth 2 -type f \( -name '*.c' -o -name '*.h' \) \
		! -path '*/src_printf/*' \
		| sort | while read -r file
	do
		echo "----- FILE: $file -----"
		cat "$file"
		echo
	done
} > "$OUT"

echo "Generated $OUT"
wc -l "$OUT"