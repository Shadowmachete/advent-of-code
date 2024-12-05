import sys
import re

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

pattern = r"mul\((\d{1,3}),(\d{1,3})\)"
do_pattern = r"do()"
dont_pattern = r"don\'t()"
p1, p2 = 0, 0
yes = True
for line in lines:
    found_match = re.search(pattern, line)
    while found_match:
        if found_match:
            mult = int(found_match.group(1)) * int(found_match.group(2))
            p1 += mult
            if yes:
                p2 += mult
        line = line[found_match.end() :]
        found_match = re.search(pattern, line)
        do_match = re.search(do_pattern, line)
        dont_match = re.search(dont_pattern, line)
        if do_match and found_match and do_match.start() < found_match.start():
            yes = True
        if dont_match and found_match and dont_match.start() < found_match.start():
            yes = False

print(p1)
print(p2)
