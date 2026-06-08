#!/bin/sh
OUT="hex_radius58_full.map"
ANTS=120
R=58
START_Q=-58
START_R=0
END_Q=58
END_R=0
abs()
{
	if [ "$1" -lt 0 ]; then
		echo $((-$1))
	else
		echo "$1"
	fi
}
inside_hex()
{
	q="$1"
	r="$2"
	s=$((-q - r))
	aq="$(abs "$q")"
	ar="$(abs "$r")"
	as="$(abs "$s")"
	if [ "$aq" -le "$R" ] && [ "$ar" -le "$R" ] && [ "$as" -le "$R" ]; then
		return 0
	fi
	return 1
}
room_name()
{
	q="$1"
	r="$2"
	if [ "$q" -eq "$START_Q" ] && [ "$r" -eq "$START_R" ]; then
		printf "START"
	elif [ "$q" -eq "$END_Q" ] && [ "$r" -eq "$END_R" ]; then
		printf "END"
	else
		if [ "$q" -lt 0 ]; then
			qs="m$((-$q))"
		else
			qs="$q"
		fi
		if [ "$r" -lt 0 ]; then
			rs="m$((-$r))"
		else
			rs="$r"
		fi
		printf "H%s_%s" "$qs" "$rs"
	fi
}
{
	printf "%d\n" "$ANTS"
	printf "# hex_radius58_full.map\n"
	printf "# axial hexagon radius 58, 10267 rooms\n"
	printf "# START logical=(-58,0), END logical=(58,0)\n"
	q=$((-R))
	while [ "$q" -le "$R" ]; do
		r=$((-R))
		while [ "$r" -le "$R" ]; do
			if inside_hex "$q" "$r"; then
				name="$(room_name "$q" "$r")"
				x=$((2 * q + r))
				y=$((2 * r))
				if [ "$q" -eq "$START_Q" ] && [ "$r" -eq "$START_R" ]; then
					printf "##start\n"
				fi
				if [ "$q" -eq "$END_Q" ] && [ "$r" -eq "$END_R" ]; then
					printf "##end\n"
				fi
				printf "%s %d %d\n" "$name" "$x" "$y"
			fi
			r=$((r + 1))
		done
		q=$((q + 1))
	done
	q=$((-R))
	while [ "$q" -le "$R" ]; do
		r=$((-R))
		while [ "$r" -le "$R" ]; do
			if inside_hex "$q" "$r"; then
				a="$(room_name "$q" "$r")"
				nq=$((q + 1))
				nr="$r"
				if inside_hex "$nq" "$nr"; then
					b="$(room_name "$nq" "$nr")"
					printf "%s-%s\n" "$a" "$b"
				fi
				nq="$q"
				nr=$((r + 1))
				if inside_hex "$nq" "$nr"; then
					b="$(room_name "$nq" "$nr")"
					printf "%s-%s\n" "$a" "$b"
				fi
				nq=$((q + 1))
				nr=$((r - 1))
				if inside_hex "$nq" "$nr"; then
					b="$(room_name "$nq" "$nr")"
					printf "%s-%s\n" "$a" "$b"
				fi
			fi
			r=$((r + 1))
		done
		q=$((q + 1))
	done
} > "$OUT"
printf "%s\n" "$OUT"