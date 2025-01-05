from aoc_tools import *

file = read_file()


#    ["7", "8", "9"]
#    ["4", "5", "6"]
#    ["1", "2", "3"]
#    ["", "0", "A"]

#    ["", "^", "A"],
#    ["<", "v", ">"]

N = [["7", "8", "9"], ["4", "5", "6"], ["1", "2", "3"], ["", "0", "A"]]

D = [["", "^", "A"], ["<", "v", ">"]]


def locate(item, grid):
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            if grid[i][j] == item:
                return (i, j)


def make_path(start, destination, grid):
    nr, nc = start[0] - destination[0], start[1] - destination[1]
    first = ""
    second = ""
    if nr < 0:
        first += "v" * abs(nr)
    elif nr > 0:
        first += "^" * nr
    if nc < 0:
        first += ">" * abs(nc)
    elif nc > 0:
        first += "<" * nc

    if nc < 0:
        second += ">" * abs(nc)
    elif nc > 0:
        second += "<" * nc
    if nr < 0:
        second += "v" * abs(nr)
    elif nr > 0:
        second += "^" * nr

    if grid[start[0]][destination[1]] == "":
        yield first
    elif grid[destination[0]][start[1]] == "":
        yield second
    else:
        yield first
        yield second


cache = {}


def solve(input, n, max_depth, v=False):
    if v:
        print("   " * (max_depth - n) + f"call {input} {n}")
    if n == 0:
        return len(input)
    if (input, n, max_depth) not in cache:
        count = 0
        if n == max_depth:
            grid = N
            current = (3, 2)
        else:
            grid = D
            current = (0, 2)
        for i in input + ("A" if n != max_depth else ""):
            destination = locate(i, grid)
            bestpath = float("inf")
            paths = make_path(current, destination, grid)
            for path in paths:
                bestpath = min(bestpath, solve(path, n - 1, max_depth))

            count += bestpath
            if n == 1:
                count += 1
            current = destination
        cache[input, n, max_depth] = count
    if v:
        print("   " * (max_depth - n) + f"-> {cache[input, n, max_depth]}")
    return cache[input, n, max_depth]

    # def make_path(input, grid, grid_type, start):
    #    print(len(input))
    #    result = ""
    #    for i in input:
    #        target = locate(i, grid)
    #        if (start, target, grid_type) in cache:
    #            temp_result = cache[(start, target, grid_type)]
    #        else:
    #            nr, nc = start[0] - target[0], start[1] - target[1]
    #            temp_result = ""
    #            if grid[start[0]][target[1]] == "":
    #                if nr < 0:
    #                    temp_result += "v" * abs(nr)
    #                elif nr > 0:
    #                    temp_result += "^" * nr
    #                if nc < 0:
    #                    temp_result += ">" * abs(nc)
    #                elif nc > 0:
    #                    temp_result += "<" * nc
    #            else:
    #                if nc < 0:
    #                    temp_result += ">" * abs(nc)
    #                elif nc > 0:
    #                    temp_result += "<" * nc
    #                if nr < 0:
    #                    temp_result += "v" * abs(nr)
    #                elif nr > 0:
    #                    temp_result += "^" * nr
    #
    #            temp_result += "A"
    #            cache[(start, target, grid_type)] = temp_result
    #        result += temp_result
    #        start = target
    #    return result


p1, p2 = 0, 0
for code in file.split("\n"):
    num = nums(code)[0]
    p1 += solve(code, 3, 3) * num
    p2 += solve(code, 26, 26) * num

print(p1)
print(p2)
