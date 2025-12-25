use std::collections::{HashMap, HashSet};
use std::fmt;
use std::str::FromStr;

#[derive(Debug, PartialEq)]
struct JunctionBox {
    x: i64,
    y: i64,
    z: i64,
}

impl JunctionBox {
    fn dist(&self, other: &JunctionBox) -> f64 {
        (((self.x - other.x).pow(2) + (self.y - other.y).pow(2) + (self.z - other.z).pow(2)) as f64)
            .sqrt()
    }
}

#[derive(Debug)]
pub struct ParseJunctionBoxError;

impl fmt::Display for ParseJunctionBoxError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Failed to parse JunctionBox struct")
    }
}

impl FromStr for JunctionBox {
    type Err = ParseJunctionBoxError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let nums: Vec<i64> = s
            .split(',')
            .map(|n| n.parse::<i64>().map_err(|_| ParseJunctionBoxError))
            .collect::<Result<_, _>>()?;

        if nums.len() != 3 {
            return Err(ParseJunctionBoxError);
        }

        Ok(JunctionBox {
            x: nums[0],
            y: nums[1],
            z: nums[2],
        })
    }
}

fn main() {
    let f = rs_aoc_tools::read_file(8);

    let num_distances_to_check = 1000;

    let boxes: Vec<JunctionBox> = f
        .lines()
        .map(|line| line.parse::<JunctionBox>().unwrap())
        .collect();

    let mut distances: Vec<(usize, usize, f64)> = Vec::new();

    for i in 0..boxes.len() {
        for j in i + 1..boxes.len() {
            distances.push((i, j, boxes[i].dist(&boxes[j])));
        }
    }

    distances.sort_by_key(|(_, _, d)| *d as i64);

    let mut circuits: Vec<HashSet<usize>> = Vec::new();
    let mut circuit_ids: HashMap<usize, usize> = HashMap::new();

    let mut part1 = 0;
    let mut part2 = 0;

    for (idx, (i, j, _)) in distances.iter().enumerate() {
        if idx == num_distances_to_check {
            let mut circuits_clone = circuits.clone();
            circuits_clone.sort_by_key(|circuit| circuit.len());

            part1 = circuits_clone[circuits_clone.len() - 1].len()
                * circuits_clone[circuits_clone.len() - 2].len()
                * circuits_clone[circuits_clone.len() - 3].len();
        }

        let &circuit_id_i = circuit_ids.get(i).unwrap_or_else(|| &usize::MAX);
        let &circuit_id_j = circuit_ids.get(j).unwrap_or_else(|| &usize::MAX);

        if circuit_id_i == usize::MAX && circuit_id_j == usize::MAX {
            let new_circuit_id = circuits.len();
            circuits.push(HashSet::new());
            circuits[new_circuit_id].insert(*i);
            circuits[new_circuit_id].insert(*j);
            circuit_ids.insert(*i, new_circuit_id);
            circuit_ids.insert(*j, new_circuit_id);
        } else if circuit_id_i == usize::MAX {
            circuits[circuit_id_j].insert(*i);
            circuit_ids.insert(*i, circuit_id_j);
        } else if circuit_id_j == usize::MAX {
            circuits[circuit_id_i].insert(*j);
            circuit_ids.insert(*j, circuit_id_i);
        } else if circuit_id_i != circuit_id_j {
            // merge 2 circuits
            let lower_circuit_id = circuit_id_i.min(circuit_id_j);
            let higher_circuit_id = circuit_id_i.max(circuit_id_j);

            let higher_circuit = circuits.remove(higher_circuit_id);

            for jbox in higher_circuit.clone() {
                circuit_ids
                    .entry(jbox)
                    .and_modify(|e| *e = lower_circuit_id);
            }

            circuits[lower_circuit_id].extend(higher_circuit);

            if higher_circuit_id < circuits.len() {
                for circuit_id in higher_circuit_id..circuits.len() {
                    let circuit = circuits[circuit_id].clone();
                    for jbox in circuit {
                        circuit_ids.entry(jbox).and_modify(|e| *e -= 1);
                    }
                }
            }
        }

        if circuits.len() == 1 && idx > num_distances_to_check && circuits[0].len() == boxes.len() {
            part2 = boxes[*i].x * boxes[*j].x;
            break;
        }
    }

    dbg!(part1);
    dbg!(part2);
}
