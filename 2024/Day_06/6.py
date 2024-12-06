import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

r = len(lines)
c = len(lines[0])

p1, p2 = 0, 0
start = (0, 0)

for i in range(r):
    for j in range(c):
        if lines[i][j] == "^":
            start = (i, j)
            break

dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)]


def traverse(block=None):
    positions = dict()
    i = 0
    dir = dirs[i]
    pos = start
    while 1:
        if pos in positions.keys() and dir in positions[pos]:
            return 1, positions

        if pos in positions.keys():
            positions[pos].append(dir)
        else:
            positions[pos] = [dir]

        if (
            (pos[0] + dir[0]) in range(r)
            and (pos[1] + dir[1]) in range(c)
            and (
                lines[pos[0] + dir[0]][pos[1] + dir[1]] in "#"
                or (
                    block
                    and pos[0] + dir[0] == block[0]
                    and pos[1] + dir[1] == block[1]
                )
            )
        ):
            i += 1
            dir = dirs[i % 4]
            continue

        if (pos[0] + dir[0]) not in range(r) or (pos[1] + dir[1]) not in range(c):
            break

        pos = (pos[0] + dir[0], pos[1] + dir[1])

    return 0, positions


_, positions = traverse()
p1 = len(positions.keys())

checked_blocks = set()
for idx, pos in enumerate(positions.keys()):
    for dir in positions[pos]:
        if (
            (pos[0] + dir[0]) not in range(r)
            or (pos[1] + dir[1]) not in range(c)
            or lines[pos[0] + dir[0]][pos[1] + dir[1]] == "#"
            or (pos[0] + dir[0], pos[1] + dir[1]) == start
        ):
            continue

        block = (pos[0] + dir[0], pos[1] + dir[1])
        if block in checked_blocks:
            continue
        checked_blocks.add(block)
        n, _ = traverse(block)
        p2 += n

print(p1)
print(p2)
