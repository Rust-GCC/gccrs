// aux-build:trait_bounds_on_structs_and_enums_xc.rs

extern crate trait_bounds_on_structs_and_enums_xc;

use trait_bounds_on_structs_and_enums_xc::{Bar, Foo, Trait};

fn main() {
    let foo = Foo {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        x: 3
    };
    let bar: Bar<f64> = return;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let _ = bar;
}

