#!/usr/bin/env bash

cat > dump.txt <<'EOF'
===== visualizer.h =====
EOF
sed -n '1,260p' includes/visualizer.h >> dump.txt

cat >> dump.txt <<'EOF'

===== draw_scene.c =====
EOF
sed -n '1,260p' srcs_bonus/visualizer/draw_scene.c >> dump.txt

cat >> dump.txt <<'EOF'

===== draw_ants.c =====
EOF
sed -n '1,260p' srcs_bonus/visualizer/draw_ants.c >> dump.txt

cat >> dump.txt <<'EOF'

===== camera.c =====
EOF
sed -n '1,260p' srcs_bonus/visualizer/camera.c >> dump.txt