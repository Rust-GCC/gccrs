// run-pass
// Test that inconsistent bounds are used in well-formedness checks
#![feature(trivial_bounds)]

use std::fmt::Debug;

pub fn foo() where Vec<str>: Debug, str: Copy {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    let x = vec![*"1"];
    println!("{:?}", x);
}

fn main() {}

