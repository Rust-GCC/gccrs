macro_rules! define_other_core {
    ( ) => {
        extern crate std as core;
// { dg-error "" "" { target *-*-* } .-1 }
    };
}

fn main() {
    core::panic!();
}

define_other_core!();

