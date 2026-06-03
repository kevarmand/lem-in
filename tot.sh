#!/usr/bin/env bash

DIR="${1:-valid_maps}"

mapfile -t MAPS < <(find "$DIR" -type f -name "*.map" | sort)

for MAP in "${MAPS[@]}"; do
	clear
	printf "\n========== %s ==========\n\n" "$MAP"
	./lem-in < "$MAP" | ./hex_visu
done