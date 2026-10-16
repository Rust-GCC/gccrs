#![allow(dead_code)]
#![warn(overflowing_literals)]

// compile-flags: -D unused-comparisons
fn main() { }


fn bar() -> i8 {
    return 123;
}

fn baz() -> bool {
    128 > bar() // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
}

