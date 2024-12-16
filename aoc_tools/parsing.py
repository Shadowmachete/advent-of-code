"""Functions for parsing data"""

import re
import sys


def read_file() -> str:
    infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
    file = open(infile).read().strip()
    return file


def grid(input: str) -> tuple[int, int, list[list[str]]]:
    split_input = [list(row) for row in input.split("\n")]
    r = len(split_input)
    c = len(split_input[0])
    return r, c, split_input


def nums(input: str) -> list[int]:
    return list(map(int, re.findall(r"[-]?\d+", input)))
