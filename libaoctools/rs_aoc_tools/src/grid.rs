use crate::{DIRS4, DIRS8};
use std::fmt;

#[derive(Debug, Clone)]
pub struct Grid {
    pub rows: usize,
    pub cols: usize,
    pub data: Vec<char>,
}

impl Grid {
    pub fn new(input: &str) -> Grid {
        let grid: Vec<Vec<char>> = input.lines().map(|line| line.chars().collect()).collect();
        Grid {
            rows: grid.len(),
            cols: grid.first().map(|r| r.len()).unwrap_or(0),
            data: grid.into_iter().flatten().collect(),
        }
    }

    pub fn rows(&self) -> usize {
        self.rows
    }

    pub fn cols(&self) -> usize {
        self.cols
    }

    pub fn idx(&self, r: usize, c: usize) -> usize {
        r * self.cols + c
    }

    pub fn in_bounds(&self, r: usize, c: usize) -> bool {
        r < self.rows && c < self.cols
    }

    pub fn get(&self, r: usize, c: usize) -> Option<&char> {
        if !self.in_bounds(r, c) {
            return None;
        }

        self.data.get(self.idx(r, c))
    }

    pub fn get_mut(&mut self, r: usize, c: usize) -> Option<&mut char> {
        if !self.in_bounds(r, c) {
            return None;
        }

        let idx = self.idx(r, c);

        self.data.get_mut(idx)
    }

    pub fn set(&mut self, r: usize, c: usize, val: char) -> bool {
        if !self.in_bounds(r, c) {
            return false;
        }

        let idx = self.idx(r, c);

        if let Some(elem) = self.data.get_mut(idx) {
            *elem = val;
            return true;
        }

        false
    }

    pub fn neighbours4(&self, r: usize, c: usize) -> impl Iterator<Item = (usize, usize)> {
        DIRS4.into_iter().filter_map(move |(dr, dc)| {
            let nr = r as isize + dr;
            let nc = c as isize + dc;
            if nr >= 0 && nc >= 0 && (nr as usize) < self.rows && (nc as usize) < self.cols {
                Some((nr as usize, nc as usize))
            } else {
                None
            }
        })
    }

    pub fn neighbours8(&self, r: usize, c: usize) -> impl Iterator<Item = (usize, usize)> {
        DIRS8.into_iter().filter_map(move |(dr, dc)| {
            let nr = r as isize + dr;
            let nc = c as isize + dc;
            if nr >= 0 && nc >= 0 && (nr as usize) < self.rows && (nc as usize) < self.cols {
                Some((nr as usize, nc as usize))
            } else {
                None
            }
        })
    }
}

impl fmt::Display for Grid {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for r in 0..self.rows {
            for c in 0..self.cols {
                write!(f, "{}", self.get(r, c).unwrap())?;
            }
            writeln!(f)?;
        }
        Ok(())
    }
}
