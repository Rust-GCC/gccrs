// aux-build:trait_bounds_on_structs_and_enums_xc.rs

extern crate trait_bounds_on_structs_and_enums_xc;

use trait_bounds_on_structs_and_enums_xc::{Bar, Foo, Trait};

fn explode(x: Foo<usize>) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn kaboom(y: Bar<f32>) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
}

