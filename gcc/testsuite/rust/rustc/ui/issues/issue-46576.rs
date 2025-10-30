// ignore-cloudabi no std::fs support

#![allow(dead_code)]
#![deny(unused_imports)]

use std::fs::File;
use std::io::{BufRead, BufReader, Read};
// { dg-error "" "" { target *-*-* } .-1 }

pub fn read_from_file(path: &str) {
    let file = File::open(&path).unwrap();
    let mut reader = BufReader::new(file);
    let mut s = String::new();
    reader.read_to_string(&mut s).unwrap();
}

pub fn read_lines(s: &str) {
    for _line in s.lines() {

    }
}

fn main() {}

