fn main() {
    let f = rs_aoc_tools::read_file(1);

    let lines: Vec<(&str, u32)> = f
        .lines()
        .map(|line| {
            let (dir, val) = line.split_at(1);
            (dir, val.parse::<u32>().unwrap())
        })
        .collect();

    let mut task1: u32 = 0;
    let mut task2: u32 = 0;
    let mut r1: u32 = 50;
    let mut r2: u32 = 50;
    let mut nval: u32;
    let mut temp: i32;

    for (dir, val) in lines.into_iter() {
        match dir {
            "R" => {
                r1 = (r1 + val) % 100;

                nval = val;
                while nval > 0 {
                    r2 = (r2 + 1) % 100;
                    task2 += (r2 == 0) as u32;
                    nval -= 1;
                }
            }
            "L" => {
                r1 = (r1 + 100 - (val % 100)) % 100;

                nval = val;
                while nval > 0 {
                    temp = r2 as i32 - 1;
                    if temp < 0 {
                        temp += 100;
                    }
                    r2 = temp as u32;
                    task2 += (r2 == 0) as u32;
                    nval -= 1;
                }
            }
            _ => unreachable!(),
        }
        task1 += (r1 == 0) as u32;
    }

    println!("Task 1: {}", task1);
    println!("Task 2: {}", task2);
}
