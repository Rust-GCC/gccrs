// Can't use unit struct as constructor function

// aux-build:empty-struct.rs

extern crate empty_struct;
use empty_struct::*;

struct Empty2;

enum E {
    Empty4
}

fn main() {
    let e2 = Empty2(); // { dg-error ".E0618." "" { target *-*-* } }
    let e4 = E::Empty4();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
    let xe2 = XEmpty2(); // { dg-error ".E0618." "" { target *-*-* } }
    let xe4 = XE::XEmpty4();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
}

