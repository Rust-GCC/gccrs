// edition:2018
// aux-build:issue-52489.rs
// compile-flags:--extern issue_52489

use issue_52489;
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

