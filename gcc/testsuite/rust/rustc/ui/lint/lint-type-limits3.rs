#![allow(dead_code)]
#![warn(overflowing_literals)]

// compile-flags: -D unused-comparisons
fn main() { }

fn qux() {
    let mut i = 1i8;
    while 200 != i { // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
        i += 1;
    }
}

