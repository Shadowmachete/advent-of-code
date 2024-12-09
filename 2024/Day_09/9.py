import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
disk_map = list(file)


def checksum(lst):
    csum = 0
    for i in range(len(lst)):
        if lst[i] == ".":
            continue
        csum += i * lst[i]
    return csum


chars = []
file_ids = []
dots = []
pos = 0
c = 0
for i in range(len(disk_map)):
    times = int(disk_map[i])
    if i % 2 == 0:
        chars.extend([c for _ in range(times)])
        file_ids.append((pos, times, c))
        c += 1
    else:
        chars.extend(["." for _ in range(times)])
        dots.append((pos, times))
    pos += times

chars2 = chars.copy()

for c in range(len(chars)):
    if c >= len(chars):
        continue
    # print(c, chars[c])
    if chars[c] != ".":
        continue
    last = chars.pop()
    while last == ".":
        last = chars.pop()
    chars[c] = last
    # print(chars)
    c += 1

for pos, size, file_id in reversed(file_ids):
    for i, (dot_pos, dot_size) in enumerate(dots):
        if dot_pos < pos and size <= dot_size:
            for j in range(size):
                assert chars2[pos + j] == file_id
                chars2[pos + j] = "."
                chars2[dot_pos + j] = file_id
            dots[i] = (dot_pos + size, dot_size - size)
            break


p1 = checksum(chars)
p2 = checksum(chars2)
print(p1)
print(p2)
