#!/usr/bin/env python3
import random
OUT = "hex_r35_gruyere_links.map"
ANTS = 120
SEED = 424242
RADIUS = 35
START = (-30, 0)
END = (30, 0)
PROTECT_RADIUS = 8
ROOM_BLOBS = 45
CRACKS = 12
BASE_LINK_CUT = 0.03
CENTER_LINK_CUT = 0.34
DIRS6 = [(1, 0), (0, 1), (-1, 1), (-1, 0), (0, -1), (1, -1)]
LINK_DIRS = [(1, 0), (0, 1), (1, -1)]
MIDPOINT_RING2_DIRS = [(DIRS6[i][0] + DIRS6[(i + 1) % 6][0], DIRS6[i][1] + DIRS6[(i + 1) % 6][1]) for i in range(6)]
def hex_dist(a, b):
    aq, ar = a
    bq, br = b
    return max(abs(aq - bq), abs(ar - br), abs((-aq - ar) - (-bq - br)))
def inside(q, r):
    return hex_dist((q, r), (0, 0)) <= RADIUS
def protected(q, r):
    return hex_dist((q, r), START) <= PROTECT_RADIUS or hex_dist((q, r), END) <= PROTECT_RADIUS
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
def add_blob(holes, cq, cr, radius):
    for q in range(cq - radius, cq + radius + 1):
        for r in range(cr - radius, cr + radius + 1):
            if inside(q, r) and not protected(q, r) and hex_dist((q, r), (cq, cr)) <= radius:
                holes.add((q, r))
def build_room_holes():
    rng = random.Random(SEED)
    cells = [(q, r) for q in range(-RADIUS, RADIUS + 1) for r in range(-RADIUS, RADIUS + 1) if inside(q, r) and not protected(q, r)]
    holes = set()
    for _ in range(ROOM_BLOBS):
        while True:
            cq, cr = rng.choice(cells)
            if rng.random() < 0.15 + 0.85 * center_factor(cq, cr):
                break
        roll = rng.random()
        if roll < 0.72:
            radius = 1
        elif roll < 0.95:
            radius = 2
        else:
            radius = 3
        add_blob(holes, cq, cr, radius)
    for _ in range(CRACKS):
        while True:
            q, r = rng.choice(cells)
            if rng.random() < 0.10 + 0.90 * center_factor(q, r):
                break
        dq, dr = rng.choice(DIRS6)
        length = rng.randint(5, 14)
        for _ in range(length):
            if inside(q, r) and not protected(q, r):
                holes.add((q, r))
                if rng.random() < 0.22:
                    bdq, bdr = rng.choice(DIRS6)
                    bq, br = q + bdq, r + bdr
                    if inside(bq, br) and not protected(bq, br):
                        holes.add((bq, br))
            if rng.random() < 0.34:
                dq, dr = rng.choice(DIRS6)
            q += dq
            r += dr
    return holes
room_holes = build_room_holes()
def exists(q, r):
    return inside(q, r) and (q, r) not in room_holes
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
def should_cut_link(aq, ar, bq, br):
    if protected(aq, ar) or protected(bq, br):
        return False
    mq = (aq + bq) / 2.0
    mr = (ar + br) / 2.0
    f = center_factor(mq, mr)
    p = BASE_LINK_CUT + CENTER_LINK_CUT * f
    return rng_links.random() < p
def add_link(aq, ar, bq, br, force=False):
    if not exists(aq, ar) or not exists(bq, br):
        return
    if not force and should_cut_link(aq, ar, bq, br):
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
for q, r, _, _, _ in rooms:
    for dq, dr in LINK_DIRS:
        add_link(q, r, q + dq, r + dr)
for q, r in (START, END):
    for dq, dr in MIDPOINT_RING2_DIRS:
        add_link(q, r, q + dq, r + dr, force=True)
with open(OUT, "w") as f:
    f.write(str(ANTS) + "\n")
    f.write("# hex_r35_gruyere_links.map\n")
    f.write("# axial hexagon radius 35\n")
    f.write("# START/END at 5 cells from border\n")
    f.write("# random room holes + random link cuts, denser near center\n")
    f.write("# START/END protected radius 8 and side-midpoint ring2 links\n")
    f.write("# seed=%d rooms=%d room_holes=%d links=%d\n" % (SEED, len(rooms), len(room_holes), len(links)))
    for q, r, n, x, y in rooms:
        if (q, r) == START:
            f.write("##start\n")
        if (q, r) == END:
            f.write("##end\n")
        f.write("%s %d %d\n" % (n, x, y))
    for a, b in links:
        f.write("%s-%s\n" % (a, b))
print("%s: %d rooms, %d room_holes, %d links" % (OUT, len(rooms), len(room_holes), len(links)))