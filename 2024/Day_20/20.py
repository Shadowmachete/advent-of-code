from aoc_tools import *
from collections import deque

file = read_file()
R, C, G = grid(file)

path = []
sr, sc = -1, -1
er, ec = -1, -1
for r in range(1, R - 1):
    for c in range(1, C - 1):
        if G[r][c] == "S":
            sr, sc = r, c
        elif G[r][c] == "E":
            er, ec = r, c
            break

path.append((sr, sc))
i = 0
while i < len(path):
    r, c = path[i]
    for dr, dc in DIRS4:
        nr, nc = r + dr, c + dc
        if (nr, nc) not in path and G[nr][nc] in "S.E":
            path.append((nr, nc))
    i += 1

assert i == len(path)
time = i
max_len = 2

p1 = 0

# cache = {}
# cheats = set()
# def solve(r, c, d, max_d, i):
#     if d == 0:
#         return 0
#     if (r, c, d) not in cache:
#         res = 0
#         for dr, dc in DIRS4:
#             nr, nc = r + dr, c + dc
#             if nr not in range(1, R - 1) or nc not in range(1, C - 1):
#                 continue
#             if (nr, nc) not in path:
#                 res += solve(r, c, d - 1, max_d, i)
#             elif path.index((nr, nc)) - (max_d - d) - i >= 100:
#
#
#     return cache[(r, c, d)]

cheats = set()
for idx, (r, c) in enumerate(path):
    if idx % 100 == 0:
        print(idx)
    visited = set()
    Q = deque()
    Q.append((r, c, 0))
    while Q:
        rr, cc, d = Q.popleft()
        if (rr, cc) in visited:
            continue
        visited.add((rr, cc))
        for dr, dc in DIRS4:
            nr, nc = rr + dr, cc + dc
            if nr not in range(1, R - 1) or nc not in range(1, C - 1):
                continue
            if d == 0:
                if (nr, nc) not in path:
                    Q.append((nr, nc, d + 1))
            else:
                if (nr, nc) in path and path.index((nr, nc)) - (d + 1) - idx >= 100:
                    if (r, c, nr, nc) not in cheats:
                        cheats.add((r, c, nr, nc))
                        if d == 1:
                            p1 += 1
                elif (nr, nc) not in path and d < max_len - 1:
                    Q.append((nr, nc, d + 1))

print(p1)
