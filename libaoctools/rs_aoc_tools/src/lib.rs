pub mod grid;
pub use grid::Grid;

use std::env;

pub const DIRS4: [(isize, isize); 4] = [(1, 0), (-1, 0), (0, 1), (0, -1)];
pub const DIRS8: [(isize, isize); 8] = [
    (-1, -1),
    (-1, 0),
    (-1, 1),
    (0, -1),
    (0, 1),
    (1, -1),
    (1, 0),
    (1, 1),
];

pub fn read_file(day: u8) -> String {
    let path = env::current_dir().expect("Cant find current directory.");
    println!("Reading file {}/inputs/{}.txt", path.display(), day);
    std::fs::read_to_string(format!("{}/inputs/{}.txt", path.display(), day))
        .expect("Cannot read input")
        .trim()
        .to_string()
}

pub fn nums(input: String) -> Vec<u32> {
    input.chars().filter_map(|c| c.to_digit(10)).collect()
}
