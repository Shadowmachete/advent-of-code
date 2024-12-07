import sys
import itertools

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

operators = ["+", "*"]
operators2 = ["+", "*", "|"]

p1 = 0
p2 = 0
for line in lines:
    ok = False
    ok2 = False
    goal, nums = line.split(": ")
    nums = list(map(int, nums.split(" ")))
    goal = int(goal)

    possibilities = [
        "".join(combo) for combo in itertools.product(operators, repeat=(len(nums) - 1))
    ]
    for possibility in possibilities:
        sum1 = nums[0]
        for i in range(len(possibility)):
            if possibility[i] == "+":
                sum1 += nums[i + 1]
            else:
                sum1 *= nums[i + 1]
        if sum1 == goal:
            ok = True
            break

    possibilities2 = [
        "".join(combo)
        for combo in itertools.product(operators2, repeat=(len(nums) - 1))
    ]
    for possibility in possibilities2:
        sum2 = nums[0]
        for i in range(len(possibility)):
            if possibility[i] == "+":
                sum2 += nums[i + 1]
            elif possibility[i] == "|":
                # sum2 = int(str(sum2) + str(nums[i + 1]))
                sum2 = sum2 * 10 ** len(str(nums[i + 1])) + nums[i + 1]
            else:
                sum2 *= nums[i + 1]
        if sum2 == goal:
            ok2 = True
            break

    if ok:
        p1 += goal

    if ok2:
        p2 += goal

print(p1)
print(p2)
