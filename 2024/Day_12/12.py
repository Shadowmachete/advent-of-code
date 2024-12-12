import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
grid = file.split("\n")

R = len(grid)
C = len(grid[0])

p1, p2 = 0, 0
visited = set()
dirs = [(0, 1), (1, 0), (-1, 0), (0, -1)]  # right, down, up, left
for r in range(R):
    for c in range(C):
        if (r, c) in visited:
            continue
        area = 0
        perimeter = 0
        perim = {}
        item = grid[r][c]
        queue = [(r, c)]
        while queue:
            r1, c1 = queue.pop(0)
            if (r1, c1) in visited:
                continue
            visited.add((r1, c1))
            area += 1
            for dx, dy in dirs:
                if (
                    r1 + dx in range(R)
                    and c1 + dy in range(C)
                    and grid[r1 + dx][c1 + dy] == item
                ):
                    queue.append((r1 + dx, c1 + dy))
                else:
                    perimeter += 1
                    if (dx, dy) not in perim:
                        perim[(dx, dy)] = set()
                    perim[(dx, dy)].add((r1, c1))
        sides = 0
        for key, values in perim.items():
            visited_perimeters = set()
            for r2, c2 in values:
                if (r2, c2) not in visited_perimeters:
                    sides += 1
                    queue = [(r2, c2)]
                    while queue:
                        r3, c3 = queue.pop(0)
                        if (r3, c3) in visited_perimeters:
                            continue
                        visited_perimeters.add((r3, c3))
                        for dx, dy in dirs:
                            if (r3 + dx, c3 + dy) in values:
                                queue.append((r3 + dx, c3 + dy))

        p1 += area * perimeter
        p2 += area * sides


print(p1)
print(p2)
