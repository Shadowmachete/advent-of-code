from aoc_tools import *
from collections import defaultdict
import heapq

file = read_file()

p1, p2 = 0, 0

R = 71
C = 71

grid = [["." for _ in range(C)] for _ in range(R)]

sr, sc = 0, 0
er, ec = 70, 70

i = 0
for line in file.split("\n"):
    if i == 1024:
        break
    x, y = nums(line)
    grid[y][x] = "#"
    i += 1

dirs = [(0, 1), (-1, 0), (0, -1), (1, 0)]


def dijkstra(grid, sr, sc, er, ec):
    dist = defaultdict(lambda: float("inf"))
    Q = []
    dist[(sr, sc)] = 0
    heapq.heappush(Q, (0, (sr, sc)))

    while Q:
        # Q = sorted(Q, key=lambda x: dist[x])
        # ur, uc = Q.pop(0)
        dis, (ur, uc) = heapq.heappop(Q)
        if (ur, uc) == (er, ec):
            break

        for dr, dc in dirs:
            vr, vc = ur + dr, uc + dc
            if vr not in range(R) or vc not in range(C):
                continue
            if grid[vr][vc] == "#":
                continue

            alt = dis + 1
            if alt < dist[(vr, vc)]:
                dist[(vr, vc)] = alt
                heapq.heappush(Q, (alt, (vr, vc)))

    return dist


# print("\n".join("".join(row) for row in grid))

dist = dijkstra(grid, sr, sc, er, ec)
p1 = dist[(er, ec)]

# linear search (at the time)
# for line in file.split("\n")[i:]:
#    x, y = nums(line)
#    grid[y][x] = "#"
#    # print("\n".join("".join(row) for row in grid))
#    dist = dijkstra(grid, sr, sc, er, ec)
#    # print(dist[(er, ec)])
#    if dist[(er, ec)] == float("inf"):
#        p2 = f"{x},{y}"
#        break

# binary search (afterwards)
file2 = file.split("\n")[i:]
left, right = 0, len(file2) - 1
while left <= right:
    mid = left + (right - left) // 2
    for j in range(mid):
        x, y = nums(file2[j])
        grid[y][x] = "#"
    dist = dijkstra(grid, sr, sc, er, ec)
    if dist[(er, ec)] == float("inf"):
        right = mid - 1
    else:
        left = mid + 1
    for j in range(mid):
        x, y = nums(file2[j])
        grid[y][x] = "."
p2 = file2[right]

print(p1)
print(p2)
