import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

r = len(lines)
c = len(lines[0])
p1, p2 = 0, 0

for i in range(r):
    for j in range(c):
        dirs = [(0, 1), (1, 0), (1, 1), (1, -1), (-1, 1), (-1, -1), (-1, 0), (0, -1)]
        for dirX, dirY in dirs:
            if (
                i + dirX * 3 < r
                and i + dirX * 3 >= 0
                and j + dirY * 3 < c
                and j + dirY * 3 >= 0
                and lines[i][j] == "X"
                and lines[i + dirX][j + dirY] == "M"
                and lines[i + dirX * 2][j + dirY * 2] == "A"
                and lines[i + dirX * 3][j + dirY * 3] == "S"
            ):
                p1 += 1

for i in range(1, r - 1):
    for j in range(1, c - 1):
        corners = [
            lines[i + 1][j + 1],
            lines[i + 1][j - 1],
            lines[i - 1][j - 1],
            lines[i - 1][j + 1],
        ]
        if lines[i][j] == "A" and "".join(corners) in ["MMSS", "SSMM", "MSSM", "SMMS"]:
            p2 += 1

print(p1)
print(p2)
