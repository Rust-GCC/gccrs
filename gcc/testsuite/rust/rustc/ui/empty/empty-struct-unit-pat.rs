// Can't use unit struct as tuple struct pattern

// aux-build:empty-struct.rs

extern crate empty_struct;
use empty_struct::*;

struct Empty2;

enum E {
    Empty4
}

fn main() {
    let e2 = Empty2;
    let e4 = E::Empty4;
    let xe2 = XEmpty2;
    let xe4 = XE::XEmpty4;

    match e2 {
        Empty2() => () // { dg-error ".E0532." "" { target *-*-* } }
    }
    match xe2 {
        XEmpty2() => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match e2 {
        Empty2(..) => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match xe2 {
        XEmpty2(..) => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }

    match e4 {
        E::Empty4() => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match xe4 {
        XE::XEmpty4() => (),
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        _ => {},
    }
    match e4 {
        E::Empty4(..) => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match xe4 {
        XE::XEmpty4(..) => (),
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        _ => {},
    }
}

