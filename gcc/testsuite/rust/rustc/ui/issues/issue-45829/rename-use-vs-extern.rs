// aux-build:issue-45829-b.rs

extern crate issue_45829_b;
use std as issue_45829_b;
// { dg-error ".E0254." "" { target *-*-* } .-1 }

fn main() {}

