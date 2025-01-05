from aoc_tools import *

file = read_file()

locks = []
keys = []

for g in file.split("\n\n"):
    g = g.split("\n")
    if g[0] == "#" * len(g[0]) and g[-1] == "." * len(g[-1]):
        l = []
        for j in range(len(g[0])):
            t = [g[i][j] for i in range(len(g))]
            l.append(t.count("#") - 1)
        locks.append(l)
    else:
        k = []
        for j in range(len(g[0])):
            t = [g[i][j] for i in range(len(g))]
            k.append(t.count("#") - 1)
        keys.append(k)

p1 = 0
for l in locks:
    for k in keys:
        if all(l[i] + k[i] <= 5 for i in range(len(l))):
            p1 += 1

print(p1)
