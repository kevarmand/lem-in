#!/bin/sh

OUT="dump.txt"

rm -f "$OUT"

{
	echo "===== TREE srcs_bonus includes ====="
	find ./srcs_bonus ./includes \
		-type f \
		| sort

	echo
	echo "===== FILES srcs_bonus includes ====="
} >> "$OUT"

find ./srcs_bonus ./includes \
	-type f \( \
		-name '*.c' -o \
		-name '*.h' \
	\) \
	| sort \
	| while read -r file
do
	{
		echo
		echo "===== $file ====="
		sed -n '1,260p' "$file"
	} >> "$OUT"
done

echo "dump written to $OUT"