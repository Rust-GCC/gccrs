// aux-build:issue-41549.rs


extern crate issue_41549;

struct S;

impl issue_41549::Trait for S {
    const CONST: () = (); // { dg-error ".E0326." "" { target *-*-* } }
}

fn main() {}

