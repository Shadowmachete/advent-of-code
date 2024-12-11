import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()

stones = [int(x) for x in file.split(" ")]
# for i in range(25):
#    new_stones = []
#    for idx in range(len(stones)):
#        n = stones[idx]
#        if n == "0":
#            new_stones.append("1")
#        elif len(n) % 2 == 1:
#            new_stones.append(str(int(n) * 2024))
#        else:
#            front, back = n[: len(n) // 2], n[len(n) // 2 :]
#            new_stones.extend([str(int(front)), str(int(back))])
#    stones = new_stones


d = {}


def count(stone, steps):
    stone_str = str(stone)
    if (stone, steps) in d:
        return d[(stone, steps)]
    if steps == 0:
        return_val = 1
    elif stone == 0:
        return_val = count(1, steps - 1)
    elif len(stone_str) % 2 == 1:
        return_val = count(stone * 2024, steps - 1)
    else:
        return_val = count(int(stone_str[: len(stone_str) // 2]), steps - 1) + count(
            int(stone_str[len(stone_str) // 2 :]), steps - 1
        )
    d[(stone, steps)] = return_val
    return return_val


print(sum(count(stone, 25) for stone in stones))
print(sum(count(stone, 75) for stone in stones))
