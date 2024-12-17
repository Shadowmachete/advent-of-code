from aoc_tools import *

file = read_file()

registers, programs = file.split("\n\n")
registers = nums(registers)
programs = nums(programs)

combos = [0, 1, 2, 3, "A", "B", "C"]
register_loc = {"A": 0, "B": 1, "C": 2}

p1, p2 = "", 0


def run(registers, programs):
    combos = [0, 1, 2, 3, "A", "B", "C"]
    register_loc = {"A": 0, "B": 1, "C": 2}
    i = 0
    out = ""
    while i < len(programs):
        opcode, operand = programs[i], programs[i + 1]
        combo_operand = combos[operand]
        if not isinstance(combo_operand, int):
            combo_operand = registers[register_loc[combo_operand]]
        # print(registers, programs)
        # print(opcode, operand)
        # print(combo_operand)
        match opcode:
            case 0:
                registers[0] //= pow(2, combo_operand)
            case 1:
                registers[1] ^= operand
            case 2:
                registers[1] = combo_operand % 8
            case 3:
                if registers[0] != 0:
                    i = operand
                    continue
            case 4:
                registers[1] ^= registers[2]
            case 5:
                out += str(combo_operand % 8) + ","
            case 6:
                registers[1] = registers[0] // pow(2, combo_operand)
            case 7:
                registers[2] = registers[0] // pow(2, combo_operand)

        i += 2
    return out


registers_copy = registers.copy()
p1 = run(registers_copy, programs)

# 2 4 B = A % 8 (last 3 bits)
# 1 6 B = B ^ 110
# 7 5 C = A // 2**B
# 4 6 B = B ^ C
# 1 4 B = B ^ 100
# 5 5 out(B)
# 0 3 A = A // 8 (3 bits moved over)
# 3 0 jnz A

# 2,4,1,6,7,5,4,6,1,4,5,5,0,3,3,0
# 245 => 3, 3, 0
# 245322 => 550330
# 245325311 => 614550330
# 245325343540 => 754614550330
# 245325343540035 => 416754614550330
# 2453253435400351 => 2416754614550330

p2 = 0o2453253435400350
while True:
    registers_copy = registers.copy()
    registers_copy[0] = p2
    output = run(registers_copy, programs)
    if output.split(",")[:-1] == programs:
        break
    if output.startswith(
        ",".join(
            [
                "2",
                "4",
                "1",
                "6",
                "7",
                "5",
                "4",
                "6",
                "1",
                "4",
                "5",
                "5",
                "0",
                "3",
                "3",
                "0",
            ]
        )
    ):
        break
    p2 += 1
print(p1[:-1])
print(p2)
