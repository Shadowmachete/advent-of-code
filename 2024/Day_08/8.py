import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

R = len(lines)
C = len(lines[0])

antennae = {}
locations = set()
locations2 = set()

# print("\n".join(lines))

for r in range(R):
    for c in range(C):
        if lines[r][c] in ".#":
            continue
        if lines[r][c] not in antennae:
            antennae[lines[r][c]] = [(r, c)]
        else:
            antennae[lines[r][c]].append((r, c))

# print(antennae)

for _, pos in antennae.items():
    n = len(pos)
    for i in range(n - 1):
        p1 = pos[i]
        for j in range(i + 1, n):
            p2 = pos[j]
            dx = p1[0] - p2[0]
            dy = p1[1] - p2[1]
            # vector is (dx, dy)
            # add +ve vector to p1 and -ve vector to p2
            # print(dx, dy)
            # print(p2[0] - dx, p2[1] - dy)
            # print(p1[0] + dx, p1[1] + dy)
            # print()
            if 0 <= p2[0] - dx < R and 0 <= p2[1] - dy < C:
                locations.add((p2[0] - dx, p2[1] - dy))
            if 0 <= p1[0] + dx < R and 0 <= p1[1] + dy < C:
                locations.add((p1[0] + dx, p1[1] + dy))

            posAr = p1[0] - dx
            posAc = p1[1] - dy
            while 0 <= posAr < R and 0 <= posAc < C:
                locations2.add((posAr, posAc))
                posAr -= dx
                posAc -= dy

            posBr = p1[0] + dx
            posBc = p1[1] + dy
            while 0 <= posBr < R and 0 <= posBc < C:
                locations2.add((posBr, posBc))
                posBr += dx
                posBc += dy

            locations2.add(p1)


p1 = len(locations)
print(p1)
p2 = len(locations2)
print(p2)
# print(locations2)
# for l1, l2 in locations2:
#     lines[l1] = lines[l1][:l2] + "#" + lines[l1][l2 + 1 :]

# print("\n".join(lines))
