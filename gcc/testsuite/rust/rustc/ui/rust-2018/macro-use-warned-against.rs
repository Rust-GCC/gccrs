// aux-build:macro-use-warned-against.rs
// aux-build:macro-use-warned-against2.rs
// check-pass

#![warn(macro_use_extern_crate, unused)]

#[macro_use] // { dg-warning "" "" { target *-*-* } }
extern crate macro_use_warned_against;
#[macro_use] // { dg-warning "" "" { target *-*-* } }
extern crate macro_use_warned_against2;

fn main() {
    foo!();
}

