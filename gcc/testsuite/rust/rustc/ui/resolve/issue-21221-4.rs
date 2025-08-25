// testing whether the lookup mechanism picks up types
// defined in the outside crate

// aux-build:issue-21221-4.rs

extern crate issue_21221_4;

struct Foo;

impl T for Foo {}
// { dg-error ".E0405." "" { target *-*-* } .-1 }

fn main() {
    println!("Hello, world!");
}

