// Test for for diagnostic improvement issue #75907, extern crate
// aux-build:issue-75907.rs

extern crate issue_75907 as a;

use a::{make_bar, Bar};

fn main() {
    let Bar(x, y, z) = make_bar();
// { dg-error ".E0532." "" { target *-*-* } .-1 }
}

