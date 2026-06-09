#!/usr/bin/env python3
OUT = "honeycomb_large.map"
ANTS = 100
RADIUS = 15
START = (-1, -11)
END = (1, 11)
STEP_X = 173
STEP_Y = 150
OFFSET_X = 1600
OFFSET_Y = 1600
DIRS = [
	(1, 0),
	(0, 1),
	(1, -1),
]
BOOST_DIRS = [
	(2, -1),
	(1, 1),
	(-1, 2),
	(-2, 1),
	(-1, -1),
	(1, -2),
]
def hex_dist(q, r):
	s = -q - r
	return max(abs(q), abs(r), abs(s))
def inside(q, r):
	return hex_dist(q, r) <= RADIUS
def name(q, r):
	if (q, r) == START:
		return "S"
	if (q, r) == END:
		return "E"
	qs = "m%d" % (-q) if q < 0 else "p%d" % q
	rs = "m%d" % (-r) if r < 0 else "p%d" % r
	return "R_%s_%s" % (qs, rs)
def coord(q, r):
	x = OFFSET_X + STEP_X * q + (STEP_X // 2) * r
	y = OFFSET_Y + STEP_Y * r
	return x, y
rooms = []
room_set = set()
coord_set = set()
for r in range(-RADIUS, RADIUS + 1):
	for q in range(-RADIUS, RADIUS + 1):
		if inside(q, r):
			n = name(q, r)
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
def add_link(aq, ar, bq, br, out, commented=False):
	if not inside(aq, ar) or not inside(bq, br):
		return
	a = name(aq, ar)
	b = name(bq, br)
	if a not in room_set or b not in room_set:
		raise RuntimeError("bad link: %s-%s" % (a, b))
	key = tuple(sorted((a, b)))
	if key in link_set:
		return
	link_set.add(key)
	if commented:
		out.append("# %s-%s" % (a, b))
	else:
		out.append("%s-%s" % (a, b))
for q, r, _, _, _ in rooms:
	for dq, dr in DIRS:
		add_link(q, r, q + dq, r + dr, links)
boost_links = []
for base in (START, END):
	q, r = base
	for dq, dr in BOOST_DIRS:
		add_link(q, r, q + dq, r + dr, boost_links, commented=True)
with open(OUT, "w") as f:
	f.write("%d\n" % ANTS)
	f.write("# honeycomb_large.map\n")
	f.write("# radius 15 axial honeycomb\n")
	f.write("# START logical=(-1,-11), END logical=(1,11)\n")
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