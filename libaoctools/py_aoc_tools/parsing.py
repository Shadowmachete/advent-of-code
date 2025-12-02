"""
Functions for parsing data
"""

import re
import sys

def read_file() -> str:
    """Reads the input file and returns it as a string."""
    infile = sys.argv[1] if len(sys.argv) >= 2 else f"{sys.argv[0].split('.')[0]}.txt"
    with open(infile, 'r', encoding='utf-8') as f:
        file = f.read().strip()
    return file


def grid(inpt: str) -> tuple[int, int, list[list[str]]]:
    """Parses a grid from the input string and returns its dimensions and content."""
    split_input = [list(row) for row in inpt.split("\n")]
    r = len(split_input)
    c = len(split_input[0])
    return r, c, split_input


def nums(inpt: str) -> list[int]:
    """Extracts all integers from the input string and returns them as a list."""
    return list(map(int, re.findall(r"[-]?\d+", inpt)))
