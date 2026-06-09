#!/usr/bin/env python3

OUT = "honeycomb_fractal_27.map"
ANTS = 100

MAP_SIDE = 33
FRACTAL_SIDE = 27

EDGE = 42
OFFSET_X = 2500
OFFSET_Y = 2500

CELL_DIRS = [
	(1, 0),
	(0, 1),
	(-1, 1),
	(-1, 0),
	(0, -1),
	(1, -1),
]

VERTEX_DIRS = [
	(1, 0),
	(0, 1),
	(-1, 1),
	(-1, 0),
	(0, -1),
	(1, -1),
]

CENTER_A = (2, -1)
CENTER_B = (1, 1)

def hex_dist(q, r):
	s = -q - r
	return max(abs(q), abs(r), abs(s))

def inside_hex(q, r, side):
	return hex_dist(q, r) <= side - 1

def cells_in_hex(center, side):
	cq, cr = center
	radius = side - 1
	for q in range(cq - radius, cq + radius + 1):
		for r in range(cr - radius, cr + radius + 1):
			if hex_dist(q - cq, r - cr) <= radius:
				yield (q, r)

def carve_fractal(center, side, holes):
	if side == 1:
		holes.add(center)
		return
	child = side // 3
	for cell in cells_in_hex(center, child):
		holes.add(cell)
	for dq, dr in CELL_DIRS:
		carve_fractal((center[0] + 2 * child * dq, center[1] + 2 * child * dr), child, holes)

def center_from_cell(q, r):
	return (
		q * CENTER_A[0] + r * CENTER_B[0],
		q * CENTER_A[1] + r * CENTER_B[1],
	)

def coord(q, r):
	x = OFFSET_X + EDGE * q + (EDGE // 2) * r
	y = OFFSET_Y + int(EDGE * 0.8660254 * r)
	return x, y

def room_name(q, r):
	if (q, r) == START:
		return "S"
	if (q, r) == END:
		return "E"
	qs = "m%d" % (-q) if q < 0 else "p%d" % q
	rs = "m%d" % (-r) if r < 0 else "p%d" % r
	return "R_%s_%s" % (qs, rs)

def dist2_to_xy(v, target):
	x, y = coord(v[0], v[1])
	tx, ty = target
	return (x - tx) * (x - tx) + (y - ty) * (y - ty)

holes = set()
carve_fractal((0, 0), FRACTAL_SIDE, holes)

cells = []
for q in range(-(MAP_SIDE - 1), MAP_SIDE):
	for r in range(-(MAP_SIDE - 1), MAP_SIDE):
		if inside_hex(q, r, MAP_SIDE) and (q, r) not in holes:
			cells.append((q, r))

vertices = set()
edges = set()

for q, r in cells:
	cq, cr = center_from_cell(q, r)
	corners = []
	for dq, dr in VERTEX_DIRS:
		corners.append((cq + dq, cr + dr))
	for corner in corners:
		vertices.add(corner)
	for i in range(6):
		edges.add(tuple(sorted((corners[i], corners[(i + 1) % 6]))))

degree = {}
for a, b in edges:
	degree[a] = degree.get(a, 0) + 1
	degree[b] = degree.get(b, 0) + 1

xs = [coord(q, r)[0] for q, r in vertices]
ys = [coord(q, r)[1] for q, r in vertices]
min_x = min(xs)
max_x = max(xs)
min_y = min(ys)
max_y = max(ys)

candidates = [v for v in vertices if degree.get(v, 0) >= 2]

target_start = (
	min_x + int((max_x - min_x) * 0.28),
	max_y - int((max_y - min_y) * 0.24),
)

START = min(candidates, key=lambda v: dist2_to_xy(v, target_start))
mirror = (-START[0], -START[1])
if mirror in vertices and degree.get(mirror, 0) >= 2:
	END = mirror
else:
	END = min(candidates, key=lambda v: dist2_to_xy(v, coord(mirror[0], mirror[1])))

rooms = []
room_set = set()
coord_set = set()

for q, r in sorted(vertices, key=lambda p: (p[1], p[0])):
	n = room_name(q, r)
	x, y = coord(q, r)
	if n in room_set:
		raise RuntimeError("duplicate room name: %s" % n)
	if (x, y) in coord_set:
		raise RuntimeError("duplicate coord: %d %d" % (x, y))
	room_set.add(n)
	coord_set.add((x, y))
	rooms.append((q, r, n, x, y))

links = []
link_set = set()

def add_link(a, b):
	aq, ar = a
	bq, br = b
	ra = room_name(aq, ar)
	rb = room_name(bq, br)
	if ra not in room_set or rb not in room_set:
		raise RuntimeError("bad link: %s-%s" % (ra, rb))
	key = tuple(sorted((ra, rb)))
	if key in link_set:
		return
	link_set.add(key)
	links.append((ra, rb))

for a, b in sorted(edges):
	add_link(a, b)

with open(OUT, "w") as f:
	f.write("%d\n" % ANTS)
	f.write("# honeycomb_fractal_27.map\n")
	f.write("# true honeycomb, fractal side 27, carrier side 33\n")
	f.write("# recursive holes: 27 -> 9 -> 3 -> 1\n")
	f.write("# cells=%d holes=%d rooms=%d links=%d\n" % (len(cells), len(holes), len(rooms), len(links)))
	f.write("# START logical=%s END logical=%s\n" % (START, END))
	for q, r, n, x, y in rooms:
		if (q, r) == START:
			f.write("##start\n")
		if (q, r) == END:
			f.write("##end\n")
		f.write("%s %d %d\n" % (n, x, y))
	for a, b in links:
		f.write("%s-%s\n" % (a, b))

print("%s: cells=%d holes=%d rooms=%d links=%d" % (OUT, len(cells), len(holes), len(rooms), len(links)))
print("START=%s END=%s" % (START, END))