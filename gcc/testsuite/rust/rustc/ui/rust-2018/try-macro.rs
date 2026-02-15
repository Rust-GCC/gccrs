// Test that `try!` macros are rewritten.

// run-rustfix
// check-pass

#![warn(rust_2018_compatibility)]
#![allow(dead_code)]
#![allow(deprecated)]

fn foo() -> Result<usize, ()> {
    let x: Result<usize, ()> = Ok(22);
    try!(x);
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    Ok(44)
}

fn main() { }

