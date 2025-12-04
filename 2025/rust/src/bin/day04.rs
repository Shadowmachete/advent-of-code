use rs_aoc_tools::Grid;

fn main() {
    let f = rs_aoc_tools::read_file(4);

    let mut grid = Grid::new(&f);
    let rows = grid.rows();
    let cols = grid.cols();

    let part1 = get_num_rolls(&mut grid, rows, cols);
    let mut part2 = part1;

    while let count = get_num_rolls(&mut grid, rows, cols)
        && count != 0
    {
        part2 += count;
    }

    println!("{}", part1);
    println!("{}", part2);
}

fn get_num_rolls(grid: &mut Grid, rows: usize, cols: usize) -> u32 {
    let mut count: u32 = 0;
    let mut num_neighbours;

    let mut remove_spots = vec![];

    for r in 0..rows {
        for c in 0..cols {
            if *grid.get(r, c).expect("never") == '.' {
                continue;
            }

            num_neighbours = 0;

            for (nr, nc) in grid.neighbours8(r, c) {
                if *grid.get(nr, nc).expect("never") == '@' {
                    num_neighbours += 1;
                }
            }

            if num_neighbours < 4 {
                count += 1;
                remove_spots.push((r, c));
            }
        }
    }

    for (r, c) in remove_spots {
        grid.set(r, c, '.');
    }

    count
}
