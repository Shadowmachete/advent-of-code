import sys
import re
import numpy as np
from numpy import linalg

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

i = 0
p1 = 0
p2 = 0
while i < len(lines):
    btn_a = lines[i]
    btn_b = lines[i + 1]
    prize = lines[i + 2]

    match = re.match(r".*: X\+(\d+), Y\+(\d+)", btn_a)
    x1, y1 = int(match.group(1)), int(match.group(2))
    match = re.match(r".*: X\+(\d+), Y\+(\d+)", btn_b)
    x2, y2 = int(match.group(1)), int(match.group(2))
    match = re.match(r".*: X\=(\d+), Y\=(\d+)", prize)
    pos1, pos2 = int(match.group(1)), int(match.group(2))

    # possibilities = []
    # for a in range(pos1 // x1 + 1):
    #    n = pos1 - x1 * a
    #    b = n // x2
    #    if b == n / x2 and (y1 * a + y2 * b) == pos2:
    #        possibilities.append((a, b))
    # possibilities = sorted(possibilities, key=lambda x: 3 * x[0] + x[1])
    # if len(possibilities) > 0 and not any(
    #    possibility[0] > 100 or possibility[1] > 100 for possibility in possibilities
    # ):
    #    p1 += possibilities[0][0] * 3 + possibilities[0][1]
    A = np.array([[x1, x2], [y1, y2]])
    P1 = np.array([pos1, pos2])
    P2 = np.array([pos1 + 10000000000000, pos2 + 10000000000000])
    X1 = linalg.solve(A, P1)
    a = round(X1[0])
    b = round(X1[1])
    if a * x1 + b * x2 == pos1 and a * y1 + b * y2 == pos2:
        p1 += 3 * a + b
    X2 = linalg.solve(A, P2)
    a = round(X2[0])
    b = round(X2[1])
    if (
        a * x1 + b * x2 == pos1 + 10000000000000
        and a * y1 + b * y2 == pos2 + 10000000000000
    ):
        p2 += 3 * a + b
    i += 4
print(p1)
print(p2)
