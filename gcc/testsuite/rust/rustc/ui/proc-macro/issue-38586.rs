// aux-build:issue-38586.rs

#[macro_use]
extern crate issue_38586;

#[derive(A)] // { dg-error ".E0425." "" { target *-*-* } }
struct A;

fn main() {}

