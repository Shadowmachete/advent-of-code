use std::fmt;
use std::iter::{Product, Sum};
use std::str::FromStr;

#[derive(Debug, PartialEq)]
enum Operation {
    Add,
    Multiply,
}

#[derive(Debug)]
pub struct ParseOperationError;

impl fmt::Display for ParseOperationError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Failed to parse Operation enum")
    }
}

impl FromStr for Operation {
    type Err = ParseOperationError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s.to_lowercase().as_str() {
            "+" => Ok(Operation::Add),
            "*" => Ok(Operation::Multiply),
            _ => Err(ParseOperationError),
        }
    }
}

impl Operation {
    fn execute<T>(&self, nums: Vec<T>) -> T
    where
        T: Copy + Sum<T> + Product<T>,
    {
        match self {
            Operation::Add => nums.into_iter().sum(),
            Operation::Multiply => nums.into_iter().product(),
        }
    }
}

fn main() {
    let f = rs_aoc_tools::read_file(6);

    let mut num_rows: Vec<Vec<u64>> = Vec::new();
    let mut operations: Vec<Operation> = Vec::new();

    f.lines().for_each(|line| {
        let nums = rs_aoc_tools::nums::<u64>(line.to_string());
        if nums.is_empty() {
            operations = line
                .split_whitespace()
                .filter_map(|op| op.parse::<Operation>().ok())
                .collect();
        } else {
            num_rows.push(nums);
        }
    });

    let char_lines: Vec<Vec<char>> = f.lines().map(|line| line.chars().collect()).collect();
    let length = char_lines[0].len();
    let mut row_count = 0;
    while char_lines[row_count][0] != '*' && char_lines[row_count][0] != '+' {
        row_count += 1;
    }
    let mut idx: usize = 0;

    let mut part1 = 0;
    let mut part2 = 0;

    for i in 0..operations.len() {
        let mut nums = Vec::new();
        for j in &num_rows {
            nums.push(j[i]);
        }

        part1 += operations[i].execute(nums);

        nums = Vec::new();
        loop {
            if idx == length {
                break;
            }
            let val = read_col(idx, &char_lines, row_count);
            idx += 1;
            if val == 0 {
                break;
            }
            nums.push(val);
        }

        part2 += operations[i].execute(nums);
    }

    dbg!(part1);
    dbg!(part2);
}

fn read_col(idx: usize, rows: &[Vec<char>], num_rows: usize) -> u64 {
    let mut tmp = 0;
    for i in 0..num_rows {
        let num = rows[i][idx].to_digit(10).unwrap_or_default();
        if num != 0 {
            tmp *= 10;
            tmp += num as u64;
        }
    }
    tmp
}
