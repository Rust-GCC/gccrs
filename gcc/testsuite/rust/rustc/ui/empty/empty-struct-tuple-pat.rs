// Can't use unit struct as enum pattern

// aux-build:empty-struct.rs

extern crate empty_struct;
use empty_struct::*;

struct Empty2();

enum E {
    Empty4()
}

// remove attribute after warning cycle and promoting warnings to errors
fn main() {
    let e2 = Empty2();
    let e4 = E::Empty4();
    let xe6 = XEmpty6();
    let xe5 = XE::XEmpty5();

    match e2 {
        Empty2 => () // { dg-error ".E0530." "" { target *-*-* } }
    }
    match xe6 {
        XEmpty6 => () // { dg-error ".E0530." "" { target *-*-* } }
    }

    match e4 {
        E::Empty4 => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match xe5 {
        XE::XEmpty5 => (),
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        _ => {},
    }
}

