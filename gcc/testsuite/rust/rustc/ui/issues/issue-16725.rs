// aux-build:issue-16725.rs

extern crate issue_16725 as foo;

fn main() {
    unsafe { foo::bar(); }
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

