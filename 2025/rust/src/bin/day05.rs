fn main() {
    let f = rs_aoc_tools::read_file(5);

    let mut fresh: Vec<(u64, u64)> = Vec::new();
    let mut part1: u16 = 0;
    let mut good = false;

    f.lines().for_each(|line| {
        if line.contains("-") {
            let range: Vec<&str> = line.split("-").collect();
            let first_num: u64 = range[0].parse::<u64>().unwrap();
            let second_num: u64 = range[1].parse::<u64>().unwrap();
            fresh.push((first_num, second_num));
        } else if !line.is_empty() {
            good = false;
            let ingredient_id: u64 = line.parse::<u64>().unwrap();
            fresh.iter().for_each(|(first, second)| {
                if ingredient_id >= *first && ingredient_id <= *second {
                    good = true;
                }
            });

            if good {
                part1 += 1;
            }
        }
    });

    dbg!(part1);

    fresh.sort_by_key(|r| r.0);

    let mut merged: Vec<(u64, u64)> = Vec::new();

    fresh.iter().for_each(|(first, second)| {
        if let Some(last) = merged.last_mut() {
            if *first <= last.1 + 1 {
                let new_end = last.1.max(*second);
                *last = (last.0, new_end);
            } else {
                merged.push((*first, *second));
            }
        } else {
            merged.push((*first, *second));
        }
    });

    let part2: u64 = merged.iter().map(|(f, s)| *s - *f + 1).sum();

    dbg!(part2);
}
