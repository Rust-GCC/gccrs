// edition:2018
// compile-flags: --extern issue_56596
// aux-build:issue-56596.rs

#![feature(uniform_paths)]

mod m {
    pub mod issue_56596 {}
}

use m::*;
use issue_56596; // { dg-error ".E0659." "" { target *-*-* } }

fn main() {}

