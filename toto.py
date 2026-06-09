#!/usr/bin/env python3
import random
OUT = "hex_r35_all_rooms_heavy_linkcuts.map"
ANTS = 120
SEED = 424241
RADIUS = 35
START = (-30, 0)
END = (30, 0)
START_END_PROTECT_RADIUS = 8
BORDER_PROTECT = 5
BASE_LINK_CUT = 0.315
CENTER_LINK_CUT = 0.585
MAX_LINK_CUT = 0.792
DIRS6 = [(1, 0), (0, 1), (-1, 1), (-1, 0), (0, -1), (1, -1)]
LINK_DIRS = [(1, 0), (0, 1), (1, -1)]
MIDPOINT_RING2_DIRS = [(DIRS6[i][0] + DIRS6[(i + 1) % 6][0], DIRS6[i][1] + DIRS6[(i + 1) % 6][1]) for i in range(6)]
def hex_dist(a, b):
    aq, ar = a
    bq, br = b
    return max(abs(aq - bq), abs(ar - br), abs((-aq - ar) - (-bq - br)))
def inside(q, r):
    return hex_dist((q, r), (0, 0)) <= RADIUS
def on_protected_border(q, r):
    return hex_dist((q, r), (0, 0)) >= RADIUS - BORDER_PROTECT
def around_start_or_end(q, r):
    return hex_dist((q, r), START) <= START_END_PROTECT_RADIUS or hex_dist((q, r), END) <= START_END_PROTECT_RADIUS
def protected(q, r):
    return on_protected_border(q, r) or around_start_or_end(q, r)
def center_factor(q, r):
    d = hex_dist((q, r), (0, 0)) / RADIUS
    f = 1.0 - d
    if f < 0.0:
        return 0.0
    return f * f
def room_name(q, r):
    if (q, r) == START:
        return "START"
    if (q, r) == END:
        return "END"
    qs = "m" + str(-q) if q < 0 else str(q)
    rs = "m" + str(-r) if r < 0 else str(r)
    return "H%s_%s" % (qs, rs)
def coord(q, r):
    return (2 * q + r, 2 * r)
def exists(q, r):
    return inside(q, r)
rooms = []
room_set = set()
coord_set = set()
for q in range(-RADIUS, RADIUS + 1):
    for r in range(-RADIUS, RADIUS + 1):
        if exists(q, r):
            n = room_name(q, r)
            x, y = coord(q, r)
            if n in room_set:
                raise RuntimeError("duplicate room name: " + n)
            if (x, y) in coord_set:
                raise RuntimeError("duplicate coord: %d %d" % (x, y))
            room_set.add(n)
            coord_set.add((x, y))
            rooms.append((q, r, n, x, y))
links = []
link_set = set()
rng_links = random.Random(SEED + 1)
cut_links = 0
kept_links = 0
def should_cut_link(aq, ar, bq, br):
    if protected(aq, ar) or protected(bq, br):
        return False
    mq = (aq + bq) / 2.0
    mr = (ar + br) / 2.0
    p = BASE_LINK_CUT + CENTER_LINK_CUT * center_factor(mq, mr)
    if p > MAX_LINK_CUT:
        p = MAX_LINK_CUT
    return rng_links.random() < p
def add_link(aq, ar, bq, br, force=False):
    global cut_links, kept_links
    if not exists(aq, ar) or not exists(bq, br):
        return
    if not force and should_cut_link(aq, ar, bq, br):
        cut_links += 1
        return
    a = room_name(aq, ar)
    b = room_name(bq, br)
    if a not in room_set or b not in room_set:
        raise RuntimeError("bad link: %s-%s" % (a, b))
    key = tuple(sorted((a, b)))
    if key in link_set:
        return
    link_set.add(key)
    links.append((a, b))
    kept_links += 1
for q, r, _, _, _ in rooms:
    for dq, dr in LINK_DIRS:
        add_link(q, r, q + dq, r + dr)
for q, r in (START, END):
    for dq, dr in MIDPOINT_RING2_DIRS:
        add_link(q, r, q + dq, r + dr, force=True)
with open(OUT, "w") as f:
    f.write(str(ANTS) + "\n")
    f.write("# hex_r35_all_rooms_heavy_linkcuts.map\n")
    f.write("# axial hexagon radius 35\n")
    f.write("# all rooms are kept, only links are cut\n")
    f.write("# full border protected with radius %d\n" % BORDER_PROTECT)
    f.write("# START/END protected radius %d and side-midpoint ring2 links\n" % START_END_PROTECT_RADIUS)
    f.write("# seed=%d rooms=%d links=%d cut_links=%d\n" % (SEED, len(rooms), len(links), cut_links))
    for q, r, n, x, y in rooms:
        if (q, r) == START:
            f.write("##start\n")
        if (q, r) == END:
            f.write("##end\n")
        f.write("%s %d %d\n" % (n, x, y))
    for a, b in links:
        f.write("%s-%s\n" % (a, b))
print("%s: %d rooms, %d links kept, %d links cut" % (OUT, len(rooms), len(links), cut_links))