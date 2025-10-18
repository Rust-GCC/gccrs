// aux-build:issue-19163.rs

#[macro_use] extern crate issue_19163;

use std::io::Write;

fn main() {
    let mut v = vec![];
    mywrite!(&v, "Hello world");
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

