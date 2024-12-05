import sys
import re

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()

p1, p2 = 0, 0

p1 += len(re.findall(r"XMAS", file)) + len(re.findall(r"SAMX", file))


lines = file.split("\n")
for _ in range(6):
    lines.insert(0, "#" * len(lines[0]))
    lines.append("#" * len(lines[0]))

lines = list(map(lambda line: 10 * "#" + line + 10 * "#", lines))

for i in range(len(lines)):
    for j in range(len(lines[i])):
        if lines[i][j] == "X":
            if lines[i + 1][j + 1] == "M":
                if lines[i + 2][j + 2] == "A":
                    if lines[i + 3][j + 3] == "S":
                        p1 += 1
            if lines[i + 1][j - 1] == "M":
                if lines[i + 2][j - 2] == "A":
                    if lines[i + 3][j - 3] == "S":
                        p1 += 1
            if lines[i - 1][j + 1] == "M":
                if lines[i - 2][j + 2] == "A":
                    if lines[i - 3][j + 3] == "S":
                        p1 += 1
            if lines[i - 1][j - 1] == "M":
                if lines[i - 2][j - 2] == "A":
                    if lines[i - 3][j - 3] == "S":
                        p1 += 1
            if lines[i + 1][j] == "M":
                if lines[i + 2][j] == "A":
                    if lines[i + 3][j] == "S":
                        p1 += 1
            if lines[i - 1][j] == "M":
                if lines[i - 2][j] == "A":
                    if lines[i - 3][j] == "S":
                        p1 += 1
        if lines[i][j] == "A":
            if (
                lines[i + 1][j + 1] == "M"
                and lines[i - 1][j + 1] == "M"
                and lines[i - 1][j - 1] == "S"
                and lines[i + 1][j - 1] == "S"
            ):
                p2 += 1
            if (
                lines[i + 1][j + 1] == "S"
                and lines[i - 1][j + 1] == "S"
                and lines[i - 1][j - 1] == "M"
                and lines[i + 1][j - 1] == "M"
            ):
                p2 += 1
            if (
                lines[i + 1][j + 1] == "S"
                and lines[i - 1][j + 1] == "M"
                and lines[i - 1][j - 1] == "M"
                and lines[i + 1][j - 1] == "S"
            ):
                p2 += 1
            if (
                lines[i + 1][j + 1] == "M"
                and lines[i - 1][j + 1] == "S"
                and lines[i - 1][j - 1] == "S"
                and lines[i + 1][j - 1] == "M"
            ):
                p2 += 1

print(p1)
print(p2)
