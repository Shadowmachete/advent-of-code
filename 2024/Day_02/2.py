import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")


def is_good(line):
    inc_or_dec = line == sorted(line) or line == sorted(line, reverse=True)
    safe = 1
    for i in range(len(line) - 1):
        if abs(line[i] - line[i + 1]) not in [1, 2, 3]:
            safe = 0
            break
    return inc_or_dec and safe


p1, p2 = 0, 0

for line in lines:
    line = list(map(int, line.split()))
    p1 += int(is_good(line))

    good = False
    for j in range(len(line)):
        line2 = line[:j] + line[j + 1 :]
        if is_good(line2):
            good = True
            break
    p2 += int(good)

print(p1)
print(p2)
