from aoc_tools import *

file = read_file()

warehouse, movements = file.split("\n\n")
R, C, G = grid(warehouse)
G2 = [[] for _ in range(R)]
sr, sc = 0, 0
sr2, sc2 = 0, 0
for r in range(R):
    for c in range(C):
        if G[r][c] == "#":
            G2[r].extend(["#", "#"])
        elif G[r][c] in ".@":
            if G[r][c] == "@":
                sr, sc = r, c
            G2[r].extend([G[r][c], "."])
        elif G[r][c] == "O":
            G2[r].extend(["[", "]"])

for r in range(R):
    for c in range(C):
        if G2[r][c] == "@":
            sr2, sc2 = r, c
            break

R2 = R
C2 = len(G2[0])
dirs = {"^": (-1, 0), "v": (1, 0), "<": (0, -1), ">": (0, 1)}

for move in list(movements):
    if move == "\n":
        continue
    dr, dc = dirs[move]
    nr, nc = sr + dr, sc + dc
    if G[nr][nc] == "#":
        continue
    collides = False
    positions_to_override = []
    while G[nr][nc] == "O":
        nr, nc = nr + dr, nc + dc
        positions_to_override.append((nr, nc))
        if G[nr][nc] == "#":
            collides = True
    if collides:
        continue
    for r, c in positions_to_override:
        G[r][c] = "O"

    G[sr][sc] = "."
    sr += dr
    sc += dc
    G[sr][sc] = "@"

for move in list(movements):
    if move == "\n":
        continue
    dr, dc = dirs[move]
    nr, nc = sr2 + dr, sc2 + dc
    if G2[nr][nc] == "#":
        continue
    collides = False
    if move in "<>" and G2[nr][nc] in "[]":
        new_positions = []
        while G2[nr][nc] in "[]":
            nc += dc
            if G2[nr][nc] == "#":
                collides = True
                break
            new_positions.append((G2[nr][nc - dc], nr, nc))
        if collides:
            continue
        for b, r, c in new_positions:
            G2[r][c] = b
    elif move in "^v" and G2[nr][nc] in "[]":
        new_positions = []
        if G2[nr][nc] == "[":
            nc2 = nc + 1
        else:
            nc2 = nc - 1
        blocks = [((G2[nr][nc], nr, nc), (G2[nr][nc2], nr, nc2))]
        while blocks:
            block = blocks.pop(0)
            for b in block:
                nr = b[1] + dr
                if G2[nr][b[2]] == "#":
                    collides = True
                    break
                new_positions.append((b[0], nr, b[2]))
                if G2[nr][b[2]] in "[]":
                    if G2[nr][b[2]] == "[":
                        nc2 = b[2] + 1
                    else:
                        nc2 = b[2] - 1
                    blocks.append(((G2[nr][b[2]], nr, b[2]), (G2[nr][nc2], nr, nc2)))
            if collides:
                break
        if collides:
            continue
        for _, r, c in new_positions:
            G2[r - dr][c] = "."
        for b, r, c in new_positions:
            G2[r][c] = b

    G2[sr2][sc2] = "."
    sr2 += dr
    sc2 += dc
    G2[sr2][sc2] = "@"

p1, p2 = 0, 0
for r in range(R):
    for c in range(C):
        if G[r][c] == "O":
            p1 += 100 * r + c
    for c in range(C2):
        if G2[r][c] == "[":
            p2 += 100 * r + c

print(p1)
print(p2)
