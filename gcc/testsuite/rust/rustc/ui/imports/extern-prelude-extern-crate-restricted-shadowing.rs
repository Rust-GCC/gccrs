// aux-build:two_macros.rs

macro_rules! define_vec {
    () => {
        extern crate std as Vec;
    }
}

define_vec!();

mod m {
    fn check() {
        Vec::panic!(); // { dg-error ".E0659." "" { target *-*-* } }
    }
}

macro_rules! define_other_core {
    () => {
        extern crate std as core;
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

define_other_core!();

fn main() {}

