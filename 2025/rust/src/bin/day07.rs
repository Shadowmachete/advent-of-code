use std::collections::HashMap;

fn main() {
    let f = rs_aoc_tools::read_file(7);

    let mut grid = rs_aoc_tools::Grid::new(&f);

    let mut beam_positions = HashMap::<usize, usize>::from([(grid.cols() / 2, 1)]);

    let mut row = 1;

    let mut part1: u16 = 0;

    while row != grid.rows() {
        let mut new_beam_positions = HashMap::<usize, usize>::new();
        for (col, count) in beam_positions {
            match grid.get(row, col) {
                Some('.') | Some('|') => {
                    place_tachyon(row, col, count, &mut grid, &mut new_beam_positions)
                }
                Some('^') => {
                    part1 += 1;
                    split_tachyon(row, col, count, &mut grid, &mut new_beam_positions)
                }
                None => unreachable!(),
                _ => unreachable!(),
            }
        }

        row += 1;

        beam_positions = new_beam_positions;
    }

    let part2 = beam_positions.values().sum::<usize>();

    dbg!(part1);
    dbg!(part2);
    // println!("{}", grid);
}

fn place_tachyon(
    r: usize,
    c: usize,
    count: usize,
    grid: &mut rs_aoc_tools::Grid,
    new_beam_positions: &mut HashMap<usize, usize>,
) {
    grid.set(r, c, '|');
    (*new_beam_positions)
        .entry(c)
        .and_modify(|value| *value += count)
        .or_insert(count);
}

fn split_tachyon(
    r: usize,
    c: usize,
    count: usize,
    grid: &mut rs_aoc_tools::Grid,
    new_beam_positions: &mut HashMap<usize, usize>,
) {
    grid.set(r, c - 1, '|');
    (*new_beam_positions)
        .entry(c - 1)
        .and_modify(|value| *value += count)
        .or_insert(count);
    grid.set(r, c + 1, '|');
    (*new_beam_positions)
        .entry(c + 1)
        .and_modify(|value| *value += count)
        .or_insert(count);
}
