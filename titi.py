#!/usr/bin/env python3

OUT = "honeycomb_large.map"
ANTS = 100
CELL_RADIUS = 8
EDGE = 70
OFFSET_X = 1400
OFFSET_Y = 1000

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

def center_from_cell(i, j):
	return (
		i * CENTER_A[0] + j * CENTER_B[0],
		i * CENTER_A[1] + j * CENTER_B[1],
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

cells = []
for i in range(-CELL_RADIUS, CELL_RADIUS + 1):
	for j in range(-CELL_RADIUS, CELL_RADIUS + 1):
		if hex_dist(i, j) <= CELL_RADIUS:
			cells.append(center_from_cell(i, j))

vertices = set()
edges = set()

for cq, cr in cells:
	corners = []
	for dq, dr in VERTEX_DIRS:
		corners.append((cq + dq, cr + dr))
	for corner in corners:
		vertices.add(corner)
	for k in range(6):
		a = corners[k]
		b = corners[(k + 1) % 6]
		edges.add(tuple(sorted((a, b))))

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

target_start = (
	min_x + int((max_x - min_x) * 0.28),
	max_y - int((max_y - min_y) * 0.25),
)

candidates = [v for v in vertices if degree.get(v, 0) == 3]

def dist2_to_target(v, target):
	x, y = coord(v[0], v[1])
	tx, ty = target
	return (x - tx) * (x - tx) + (y - ty) * (y - ty)

START = min(candidates, key=lambda v: dist2_to_target(v, target_start))
mirror = (-START[0], -START[1])

if mirror in vertices and degree.get(mirror, 0) == 3:
	END = mirror
else:
	END = min(candidates, key=lambda v: dist2_to_target(v, coord(mirror[0], mirror[1])))

room_set = set()
coord_set = set()
rooms = []

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

def add_link(a, b, out, commented=False):
	aq, ar = a
	bq, br = b
	ra = room_name(aq, ar)
	rb = room_name(bq, br)
	if ra not in room_set or rb not in room_set:
		return
	key = tuple(sorted((ra, rb)))
	if key in link_set and not commented:
		return
	if commented:
		out.append("# %s-%s" % (ra, rb))
	else:
		link_set.add(key)
		out.append("%s-%s" % (ra, rb))

for a, b in sorted(edges):
	add_link(a, b, links)

adj = {}
for a, b in edges:
	adj.setdefault(a, set()).add(b)
	adj.setdefault(b, set()).add(a)

boost_links = []
boost_seen = set()

for base in (START, END):
	second_ring = set()
	for n1 in adj.get(base, set()):
		for n2 in adj.get(n1, set()):
			if n2 != base and n2 not in adj.get(base, set()):
				second_ring.add(n2)
	for target in sorted(second_ring, key=lambda v: dist2_to_target(v, coord(base[0], base[1])))[:6]:
		key = tuple(sorted((room_name(base[0], base[1]), room_name(target[0], target[1]))))
		if key not in boost_seen:
			boost_seen.add(key)
			add_link(base, target, boost_links, commented=True)

with open(OUT, "w") as f:
	f.write("%d\n" % ANTS)
	f.write("# honeycomb_large.map\n")
	f.write("# true honeycomb graph: hex cells, degree about 3\n")
	f.write("# CELL_RADIUS=%d EDGE=%d rooms=%d links=%d\n" % (CELL_RADIUS, EDGE, len(rooms), len(links)))
	f.write("# START logical=%s END logical=%s\n" % (START, END))
	f.write("# boosted links are commented at the end\n")
	for q, r, n, x, y in rooms:
		if (q, r) == START:
			f.write("##start\n")
		if (q, r) == END:
			f.write("##end\n")
		f.write("%s %d %d\n" % (n, x, y))
	for line in links:
		f.write("%s\n" % line)
	f.write("# BOOSTED_LINKS_START\n")
	for line in boost_links:
		f.write("%s\n" % line)
	f.write("# BOOSTED_LINKS_END\n")

print("%s: %d rooms, %d links, %d commented boost links" % (OUT, len(rooms), len(links), len(boost_links)))
print("START=%s END=%s" % (START, END))