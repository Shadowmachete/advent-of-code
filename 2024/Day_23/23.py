from aoc_tools import *
import networkx as nx
from collections import defaultdict
from itertools import permutations

file = read_file()

G = nx.Graph()
connections = defaultdict(list)
for l in file.split("\n"):
    a, b = l.split("-")
    G.add_edge(a, b)
    connections[a].append(b)
    connections[b].append(a)


p1 = 0
seen = set()
for a, vs in connections.items():
    for b in vs:
        for c in connections[b]:
            if any(perm in seen for perm in permutations((a, b, c))):
                continue
            seen.add((a, b, c))
            if c in connections[a] and any(x.startswith("t") for x in (a, b, c)):
                p1 += 1

print(p1)
max_clique = nx.approximation.max_clique(G)
print(",".join(sorted(max_clique)))
