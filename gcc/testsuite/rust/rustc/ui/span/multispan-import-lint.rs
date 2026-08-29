// check-pass

#![warn(unused)]

use std::cmp::{Eq, Ord, min, PartialEq, PartialOrd};
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {
    let _ = min(1, 2);
}

