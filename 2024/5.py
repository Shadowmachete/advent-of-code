import sys

infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
file = open(infile).read().strip()
lines = file.split("\n")
rules = lines[: lines.index("")]
updates = lines[lines.index("") + 1 :]


def correct(nums):
    for i in range(0, len(nums) - 1):
        for j in range(i + 1, len(nums)):
            if (nums[j], nums[i]) in rulebook:
                return False
    return True


def reorder(nums):
    for i in range(0, len(nums) - 1):
        for j in range(i + 1, len(nums)):
            if (nums[j], nums[i]) in rulebook:
                nums[i], nums[j] = nums[j], nums[i]
    return nums


rulebook = set()
p1, p2 = 0, 0
for rule in rules:
    n1, n2 = rule.split("|")
    rulebook.add((int(n1), int(n2)))

for update in updates:
    nums = list(map(int, update.split(",")))
    if correct(nums):
        p1 += nums[len(nums) // 2]
    else:
        nums = reorder(nums)
        p2 += nums[len(nums) // 2]

print(p1)
print(p2)
