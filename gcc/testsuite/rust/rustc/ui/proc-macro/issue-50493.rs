// aux-build:issue-50493.rs

#[macro_use]
extern crate issue_50493;

#[derive(Derive)]
struct Restricted {
    pub(in restricted) field: usize, // { dg-error ".E0742." "" { target *-*-* } }
}

mod restricted {}

fn main() {}

