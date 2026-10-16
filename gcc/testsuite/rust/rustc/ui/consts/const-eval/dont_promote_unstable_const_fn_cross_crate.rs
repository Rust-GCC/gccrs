// aux-build:stability.rs

extern crate stability;

use stability::foo;

fn main() {
    let _: &'static u32 = &foo(); // { dg-error ".E0716." "" { target *-*-* } }
    let _x: &'static u32 = &foo(); // { dg-error ".E0716." "" { target *-*-* } }
}

