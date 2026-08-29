// edition:2018
// aux-build:removing-extern-crate.rs
// run-rustfix
// check-pass

#![warn(rust_2018_idioms)]

extern crate removing_extern_crate as foo; // { dg-warning "" "" { target *-*-* } }
extern crate core; // { dg-warning "" "" { target *-*-* } }

mod another {
    extern crate removing_extern_crate as foo; // { dg-warning "" "" { target *-*-* } }
    extern crate core; // { dg-warning "" "" { target *-*-* } }
}

fn main() {}

