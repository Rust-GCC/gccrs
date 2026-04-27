// aux-build:trait_safety_lib.rs

// Check that unsafe traits require unsafe impls and that inherent
// impls cannot be unsafe.

extern crate trait_safety_lib as lib;

struct Bar;
impl lib::Foo for Bar { // { dg-error ".E0200." "" { target *-*-* } }
    fn foo(&self) -> isize {
        panic!();
    }
}

fn main() { }

