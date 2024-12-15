import sys
import re
from collections import defaultdict

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

bots = defaultdict(list)
p1bots = defaultdict(list)

for line in lines:
    p, v = line.split(" ")
    px, py = list(map(int, re.findall(r"[-]?\d+", p)))
    vx, vy = list(map(int, re.findall(r"[-]?\d+", v)))
    bots[(px, py)].append((vx, vy))

p2 = 0
for i in range(1, 10000):
    new_bots = defaultdict(list)
    grid = [["." for _ in range(101)] for _ in range(103)]
    for pos, bot_list in bots.items():
        for bot in bot_list:
            ipx, ipy = pos
            vx, vy = bot
            npx, npy = ipx + vx, ipy + vy
            if npx < 0:
                npx = (101 + npx) % 101
            elif npx > 100:
                npx = abs(101 - npx) % 101
            if npy < 0:
                npy = (103 + npy) % 103
            elif npy > 102:
                npy = abs(103 - npy) % 103
            # if npx < 0:
            #    npx = (11 + npx) % 11
            # elif npx > 10:
            #    npx = abs(11 - npx) % 11
            # if npy < 0:
            #    npy = (7 + npy) % 7
            # elif npy > 6:
            #    npy = abs(7 - npy) % 7
            new_bots[(npx, npy)].append((vx, vy))
            grid[npy][npx] = "#"
    if i % 100 == 0:
        print(i)

    SEEN = set()
    count = 0
    for pos in bots.keys():
        if pos in SEEN:
            continue
        new_count = 0
        queue = [pos]
        while queue:
            px, py = queue.pop(0)
            if (px, py) in SEEN:
                continue
            new_count += 1

            SEEN.add((px, py))
            for dx, dy in [(0, 1), (1, 0), (-1, 0), (0, -1)]:
                if (
                    px + dx in range(101)
                    and py + dy in range(103)
                    and grid[py][px] == "#"
                ):
                    queue.append((px + dx, py + dy))
        count = max(count, new_count)
    if count > 50:
        print(i)
        print("\n".join(["".join(row) for row in grid]))
    bots = new_bots
    if i == 100:
        p1bots = bots

q1 = 0
q2 = 0
q3 = 0
q4 = 0
for pos, bot_list in p1bots.items():
    if pos[0] < 50 and pos[1] < 51:
        q1 += len(bot_list)
    elif pos[0] > 50 and pos[1] < 51:
        q2 += len(bot_list)
    elif pos[0] > 50 and pos[1] > 51:
        q3 += len(bot_list)
    elif pos[0] < 50 and pos[1] > 51:
        q4 += len(bot_list)
    # if pos[0] < 5 and pos[1] < 3:
    #    q1 += len(bot_list)
    # elif pos[0] > 5 and pos[1] < 3:
    #    q2 += len(bot_list)
    # elif pos[0] > 5 and pos[1] > 3:
    #    q3 += len(bot_list)
    # elif pos[0] < 5 and pos[1] > 3:
    #    q4 += len(bot_list)

p1 = q1 * q2 * q3 * q4
print(p1)
