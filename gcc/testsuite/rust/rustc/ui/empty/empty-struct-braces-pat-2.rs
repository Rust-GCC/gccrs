// Can't use empty braced struct as enum pattern

// aux-build:empty-struct.rs

extern crate empty_struct;
use empty_struct::*;

struct Empty1 {}

fn main() {
    let e1 = Empty1 {};
    let xe1 = XEmpty1 {};

    match e1 {
        Empty1() => () // { dg-error ".E0532." "" { target *-*-* } }
    }
    match xe1 {
        XEmpty1() => () // { dg-error ".E0532." "" { target *-*-* } }
    }
    match e1 {
        Empty1(..) => () // { dg-error ".E0532." "" { target *-*-* } }
    }
    match xe1 {
        XEmpty1(..) => () // { dg-error ".E0532." "" { target *-*-* } }
    }
}

