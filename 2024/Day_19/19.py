from aoc_tools import *
from functools import cache

file = read_file()

towel, patterns = file.split("\n\n")
towels = towel.split(", ")


@cache
def solve(pattern):
    if len(pattern) == 0:
        return True
    sub_patterns = []
    for towel in towels:
        if pattern.startswith(towel):
            sub_patterns.append(pattern[len(towel) :])
    return sum(solve(sub_pattern) for sub_pattern in sub_patterns)


p1, p2 = 0, 0
for line in patterns.split("\n"):
    res = solve(line)
    if res > 0:
        p1 += 1
    p2 += res

print(p1)
print(p2)
