// aux-build:struct_field_privacy.rs

extern crate struct_field_privacy as xc;

use xc::B;

struct A {
    pub a: u32,
    b: u32,
}

fn main () {
    // external crate struct
    let k = B {
        aa: 20,
// { dg-error ".E0560." "" { target *-*-* } .-1 }
        bb: 20,
// { dg-error ".E0560." "" { target *-*-* } .-1 }
    };
    // local crate struct
    let l = A {
        aa: 20,
// { dg-error ".E0560." "" { target *-*-* } .-1 }
        bb: 20,
// { dg-error ".E0560." "" { target *-*-* } .-1 }
    };
}

