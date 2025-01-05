from aoc_tools import *
from collections import defaultdict
import re

file = read_file()

a, b = file.split("\n\n")

wires = defaultdict()
waiting = []

for l in a.split("\n"):
    w, v = l.split(": ")
    wires[w] = int(v)


def process(w1, w2, w3, op):
    if op == "XOR":
        wires[w3] = wires[w1] ^ wires[w2]
    elif op == "AND":
        wires[w3] = wires[w1] & wires[w2]
    else:
        wires[w3] = wires[w1] | wires[w2]


for l in b.split("\n"):
    m = re.match(r"(\w+) (\w+) (\w+) -> (\w+)", l)
    w1, op, w2, w3 = m.groups()
    if all(w in wires for w in (w1, w2)):
        process(w1, w2, w3, op)
    else:
        waiting.append((w1, w2, w3, op))


while waiting:
    waiting = sorted(
        waiting, key=lambda w: w[0] in wires and w[1] in wires, reverse=True
    )
    w1, w2, w3, op = waiting.pop(0)
    process(w1, w2, w3, op)

zs = sorted(filter(lambda x: x[0].startswith("z"), wires.items()))
p1 = 0
for idx, (_, bit) in enumerate(zs):
    p1 += bit * (2**idx)

print(p1)
