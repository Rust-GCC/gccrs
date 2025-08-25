// Test that an assignment of type ! makes the rest of the block dead code.

// check-pass

#![feature(never_type)]
#![warn(unused)]

fn main() {
    let x: ! = panic!("aah"); // { dg-warning "" "" { target *-*-* } }
    drop(x); // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
}

