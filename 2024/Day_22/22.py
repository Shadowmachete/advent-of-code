from aoc_tools import *
from collections import defaultdict

file = read_file()

p1 = 0
prices = []
changes = []
for idx, num in enumerate(file.split("\n")):
    n = int(num)
    prices.append([])
    changes.append([])
    for i in range(2000):
        prices[idx].append(n % 10)
        n ^= n * 64
        n %= 16777216

        n ^= n // 32
        n %= 16777216

        n ^= n * 2048
        n %= 16777216
        changes[idx].append(prices[idx][i] - prices[idx][i - 1])
    p1 += n

print(p1)

scores = defaultdict(int)
for idx, (p, c) in enumerate(zip(prices, changes)):
    seen = set()
    for i in range(1, 2000 - 4):
        seq = tuple(c[i : i + 4])
        if seq not in seen:
            scores[seq] += p[i + 3]
            seen.add(seq)

p2 = max(scores.values())

print(p2)
