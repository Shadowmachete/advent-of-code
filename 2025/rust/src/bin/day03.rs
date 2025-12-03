fn main() {
    let f = rs_aoc_tools::read_file(3);

    let mut task1 = 0;
    let mut task2 = 0;

    f.lines().for_each(|line| {
        let bank: Vec<u32> = rs_aoc_tools::nums(line.to_string());

        task1 += max_joltage_p1(&bank);
        task2 += max_joltage_p2(&bank);
    });

    println!("{}", task1);
    println!("{}", task2);
}

fn max_joltage_p1(bank: &[u32]) -> u32 {
    let mut first_pos = 0;
    for i in 0..bank.len() - 1 {
        if bank[i] > bank[first_pos] {
            first_pos = i;
        }
    }

    let mut second_pos = first_pos + 1;
    for i in first_pos + 1..bank.len() {
        if bank[i] > bank[second_pos] {
            second_pos = i;
        }
    }

    bank[first_pos] * 10 + bank[second_pos]
}

fn max_joltage_p2(bank: &[u32]) -> u64 {
    let mut indices: [usize; 12] = [0; 12];

    for i in 0..12 {
        if i != 0 {
            indices[i] = find_max(bank, 12 - i - 1, indices[i - 1] + 1);
        } else {
            indices[i] = find_max(bank, 11, 0);
        }
    }

    let mut val = 0;
    indices
        .into_iter()
        .for_each(|idx| val = val * 10 + bank[idx] as u64);
    val
}

fn find_max(bank: &[u32], spaces_left: usize, next_index: usize) -> usize {
    let mut pos = next_index;
    for i in next_index..bank.len() - spaces_left {
        if bank[i] > bank[pos] {
            pos = i;
        }
    }
    pos
}
