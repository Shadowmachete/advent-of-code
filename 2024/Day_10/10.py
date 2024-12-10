import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
grid = file.split("\n")
grid = [list(map(int, line)) for line in grid]

p1, p2 = 0, 0

R = len(grid)
C = len(grid[0])

dirs = [(-1, 0), (1, 0), (0, -1), (0, 1)]


def traverse(r, c, found):
    if grid[r][c] == 9 and (r, c) in found:
        return 0, 1, found
    elif grid[r][c] == 9:
        found.append((r, c))
        return 1, 1, found

    n1, n2 = 0, 0
    for dir in dirs:
        nr, nc = r + dir[0], c + dir[1]
        if 0 <= nr < R and 0 <= nc < C and grid[nr][nc] - grid[r][c] == 1:
            a, b, found = traverse(nr, nc, found)
            n1 += a
            n2 += b
    return n1, n2, found


for r in range(R):
    for c in range(C):
        if grid[r][c] == 0:
            found = []
            n1, n2, _ = traverse(r, c, found)
            p1 += n1
            p2 += n2

print(p1)
print(p2)
