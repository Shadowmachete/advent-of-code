from aoc_tools import *
from collections import defaultdict
import heapq

file = read_file()

R, C, G = grid(file)

sr, sc = 0, 0
er, ec = 0, 0
for r in range(R):
    for c in range(C):
        if G[r][c] == "S":
            sr = r
            sc = c
            break
        elif G[r][c] == "E":
            er = r
            ec = c

dirs = [(0, 1), (-1, 0), (0, -1), (1, 0)]


def dijkstra(grid, sr, sc, er, ec):
    dist = defaultdict(lambda: float("inf"))
    prev = defaultdict(list)
    Q = []
    dist[(sr, sc, 0)] = 0
    heapq.heappush(Q, (0, (sr, sc, 0)))

    while Q:
        # Q = sorted(Q, key=lambda x: dist[x])
        # ur, uc = Q.pop(0)
        dis, (ur, uc, curr_dir) = heapq.heappop(Q)
        if (ur, uc) == (er, ec):
            continue

        for nd, (dr, dc) in enumerate(dirs):
            vr, vc = ur + dr, uc + dc
            if grid[vr][vc] == "#":
                continue

            alt = dis + 1
            if nd != curr_dir:
                alt += 1000
            if alt < dist[(vr, vc, nd)]:
                dist[(vr, vc, nd)] = alt
                prev[(vr, vc, nd)] = [(ur, uc, curr_dir)]
                heapq.heappush(Q, (alt, (vr, vc, nd)))
            elif alt == dist[(vr, vc, nd)]:
                prev[(vr, vc, nd)].append((ur, uc, curr_dir))

    return dist, prev


distances, previous = dijkstra(G, sr, sc, er, ec)

# print(previous)
p1 = float("inf")
for d in range(4):
    if distances[(er, ec, d)] < p1:
        p1 = distances[(er, ec, d)]


def backtrack(curr, sr, sc):
    if curr[0] == sr and curr[1] == sc:
        return [(curr[0], curr[1])]

    nodes = []
    for pre in previous[curr]:
        n = backtrack(pre, sr, sc)
        nodes.extend(n + [(curr[0], curr[1])])
    return nodes


path_nodes = []
for d in range(4):
    if distances[(er, ec, d)] != p1:
        continue
    if (er, ec, d) in previous:
        path_nodes.extend(backtrack((er, ec, d), sr, sc))
    # stack = [(er, ec, d)]
    # path_nodes.add((er, ec))
    # while stack:
    #    r, c, d = stack.pop(-1)
    #    for pnode in previous[(r, c, d)]:
    #        pr, pc, pd = pnode
    #        if (pr, pc) not in path_nodes:
    #            path_nodes.add((pr, pc))
    #            stack.append(pnode)


p2 = len(set(path_nodes))

for r, c in set(path_nodes):
    if G[r][c] == ".":
        G[r][c] = "O"


# print(G)
# print(sr, sc)
# print(er, ec)

print("\n".join("".join(row) for row in G))
print(p1)
print(p2)
