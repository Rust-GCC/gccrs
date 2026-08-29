// Can't use empty braced struct as constant or constructor function

// aux-build:empty-struct.rs

extern crate empty_struct;
use empty_struct::*;

struct Empty1 {}

enum E {
    Empty3 {}
}

fn main() {
    let e1 = Empty1; // { dg-error ".E0423." "" { target *-*-* } }
    let e1 = Empty1();
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let e3 = E::Empty3; // { dg-error ".E0423." "" { target *-*-* } }
    let e3 = E::Empty3();
// { dg-error ".E0423." "" { target *-*-* } .-1 }

    let xe1 = XEmpty1; // { dg-error ".E0423." "" { target *-*-* } }
    let xe1 = XEmpty1();
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let xe3 = XE::Empty3; // { dg-error ".E0599." "" { target *-*-* } }
    let xe3 = XE::Empty3(); // { dg-error ".E0599." "" { target *-*-* } }

    XE::Empty1 {}; // { dg-error ".E0599." "" { target *-*-* } }
}

