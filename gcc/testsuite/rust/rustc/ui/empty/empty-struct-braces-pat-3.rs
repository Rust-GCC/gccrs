// Can't use empty braced struct as enum pattern

// aux-build:empty-struct.rs

extern crate empty_struct;
use empty_struct::*;

enum E {
    Empty3 {}
}

fn main() {
    let e3 = E::Empty3 {};
    let xe3 = XE::XEmpty3 {};

    match e3 {
        E::Empty3() => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match xe3 {
        XE::XEmpty3() => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match e3 {
        E::Empty3(..) => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
    match xe3 {
        XE::XEmpty3(..) => ()
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
}

