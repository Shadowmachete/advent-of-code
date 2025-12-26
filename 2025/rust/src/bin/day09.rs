use std::collections::HashMap;

#[derive(Debug, PartialEq, Eq, Hash, Clone)]
struct Tile {
    r: u64,
    c: u64,
}

fn main() {
    let f = rs_aoc_tools::read_file(9);

    let tiles: Vec<Tile> = f
        .lines()
        .map(|line| {
            let mut spl = line.split(",");
            let c = spl.next().unwrap().parse::<u64>().unwrap();
            let r = spl.next().unwrap().parse::<u64>().unwrap();
            Tile { r, c }
        })
        .collect();

    let mut part1 = 0;
    let mut part2 = 0;

    let mut seen = HashMap::new();

    for i in 0..tiles.len() {
        for j in i + 1..tiles.len() {
            let rect_area = area(&tiles[i], &tiles[j]);
            part1 = part1.max(rect_area);

            if rect_area <= part2 {
                continue;
            }

            let r_min = tiles[i].r.min(tiles[j].r);
            let r_max = tiles[i].r.max(tiles[j].r);
            let c_min = tiles[i].c.min(tiles[j].c);
            let c_max = tiles[i].c.max(tiles[j].c);

            // find other 2 corners and check if in the region
            let corner1 = Tile {
                r: tiles[i].r,
                c: tiles[j].c,
            };
            let corner2 = Tile {
                r: tiles[j].r,
                c: tiles[i].c,
            };

            if in_region(&corner1, &tiles, &mut seen) && in_region(&corner2, &tiles, &mut seen) {
                let mut valid = true;

                // check that no edge cuts through the rectangle

                for k in 0..tiles.len() {
                    let a = &tiles[k];
                    let b = &tiles[(k + 1) % tiles.len()];

                    if a.r == b.r {
                        // check if a horizontal edge cuts through the rectangle
                        if a.r > r_min && a.r < r_max {
                            let (edge_c_min, edge_c_max) = (a.c.min(b.c), a.c.max(b.c));
                            if edge_c_max > c_min && edge_c_min < c_max {
                                valid = false;
                                break;
                            }
                        }
                    } else if a.c > c_min && a.c < c_max {
                        // check if a vertical edge cuts through the rectangle
                        let (edge_r_min, edge_r_max) = (a.r.min(b.r), a.r.max(b.r));
                        if edge_r_max > r_min && edge_r_min < r_max {
                            valid = false;
                            break;
                        }
                    }
                }

                if valid {
                    part2 = part2.max(rect_area);
                }
            }
        }
    }

    dbg!(part1);
    dbg!(part2);
}

fn area(t1: &Tile, t2: &Tile) -> u64 {
    (t1.r.abs_diff(t2.r) + 1) * (t1.c.abs_diff(t2.c) + 1)
}

fn in_region(tile: &Tile, tiles: &[Tile], seen: &mut HashMap<Tile, bool>) -> bool {
    if let Some(&value) = seen.get(tile) {
        return value;
    }

    for i in 0..tiles.len() {
        let a = &tiles[i];
        let b = &tiles[(i + 1) % tiles.len()];

        if on_edge(tile, a, b) || tile == a {
            seen.insert(tile.clone(), true);
            return true;
        }
    }

    let mut counter = 0;

    for i in 0..tiles.len() {
        let a = &tiles[i];
        let b = &tiles[(i + 1) % tiles.len()];

        if a.c == b.c {
            continue;
        }

        let (c_min, c_max) = if a.c < b.c { (a.c, b.c) } else { (b.c, a.c) };

        if tile.c >= c_min && tile.c < c_max && tile.r > a.r {
            counter += 1;
        }
    }

    seen.insert(tile.clone(), counter % 2 == 1);

    counter % 2 == 1
}

fn on_edge(tile: &Tile, a: &Tile, b: &Tile) -> bool {
    if a.r == b.r {
        // along a horizontal edge
        let (c_min, c_max) = if a.c < b.c { (a.c, b.c) } else { (b.c, a.c) };
        tile.r == a.r && tile.c >= c_min && tile.c <= c_max
    } else {
        // along a vertical edge
        let (r_min, r_max) = if a.r < b.r { (a.r, b.r) } else { (b.r, a.r) };
        tile.c == a.c && tile.r >= r_min && tile.r <= r_max
    }
}
