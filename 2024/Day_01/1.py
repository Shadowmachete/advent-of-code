import sys
from collections import Counter

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

LEFT, RIGHT = [], []
RIGHTCOUNT = Counter()

for line in lines:
    l, r = line.split("   ")
    LEFT.append(int(l))
    RIGHT.append(int(r))
    RIGHTCOUNT[int(r)] += 1

p1 = 0
LEFT = sorted(LEFT)
RIGHT = sorted(RIGHT)
for l, r in zip(LEFT, RIGHT):
    p1 += abs(r - l)
print(p1)

p2 = 0
for l in LEFT:
    p2 += RIGHTCOUNT[l] * l
print(p2)
