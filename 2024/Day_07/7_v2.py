import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")

operators = ["+", "*"]
operators2 = ["+", "*", "|"]

p1 = 0
p2 = 0
for line in lines:
    ok1 = False
    ok2 = False
    goal, nums = line.split(": ")
    nums = list(map(int, nums.split(" ")))
    goal = int(goal)

    power = len(nums) - 1
    n2 = 2**power
    n3 = 3**power

    for num in range(n2):
        i = 1
        sum1 = nums[0]
        while (power - i) >= 0:
            quotient = num // (2 ** (power - i))
            if operators[quotient] == "+":
                sum1 += nums[i]
            else:
                sum1 *= nums[i]
            num %= 2 ** (power - i)
            i += 1
        if sum1 == goal:
            ok1 = True
            break

    for num in range(n3):
        i = 1
        sum2 = nums[0]
        while (power - i) >= 0:
            quotient = num // (3 ** (power - i))
            if operators2[quotient] == "+":
                sum2 += nums[i]
            elif operators2[quotient] == "|":
                sum2 = int(str(sum2) + str(nums[i]))
            else:
                sum2 *= nums[i]
            num %= 3 ** (power - i)
            i += 1
        if sum2 == goal:
            ok2 = True
            break

    if ok1:
        p1 += goal
    if ok2:
        p2 += goal

print(p1)
print(p2)
