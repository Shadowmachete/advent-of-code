use std::ops::Range;

fn main() {
    let f = rs_aoc_tools::read_file(2);

    let ranges: Vec<Range<u64>> = f
        .split(',')
        .map(|s| {
            let ids: Vec<u64> = s
                .split('-')
                .map(|i| {
                    i.parse::<u64>()
                        .unwrap_or_else(|_| panic!("Couldnt parse {}", i))
                })
                .collect();

            Range {
                start: ids[0],
                end: ids[1] + 1,
            }
        })
        .collect();

    let mut task1 = 0;
    let mut task2 = 0;

    ranges.iter().for_each(|range| {
        for num in range.start..range.end {
            let length = num.to_string().chars().count() as u8;

            // if num / 10u32.pow(length / 2) == num % 10u32.pow(length / 2) {
            //    task1 += num;
            // }

            if length.is_multiple_of(2) && compare_segments(num, length, 2) == 0 {
                task1 += num;
            }

            for factor in 2..=length {
                if length.is_multiple_of(factor) && compare_segments(num, length, factor) == 0 {
                    task2 += num;
                    break;
                }
            }
        }
    });

    println!("{}", task1);
    println!("{}", task2);
}

// new method
fn compare_segments(num: u64, length: u8, factor: u8) -> u8 {
    let segment_length = length / factor;
    let p = 10u64.pow(segment_length as u32);

    let first = num % p;
    let mut copy = num / p;

    for _ in 1..factor {
        if copy % p != first {
            return 1;
        }
        copy /= p;
    }
    0
}

// original bad method
// fn compare_substrings(numschar: &[char], length: u8, factor: u8) -> u8 {
//     let segment_length: usize = (length / factor) as usize;
//     let num_segments = factor as usize;
//
//     let mut first;
//     for i in 0..segment_length {
//         first = numschar[i];
//         for j in 1..num_segments {
//             let idx = j * segment_length + i;
//
//             if numschar[idx] != first {
//                 return 1;
//             }
//         }
//     }
//     0
// }
