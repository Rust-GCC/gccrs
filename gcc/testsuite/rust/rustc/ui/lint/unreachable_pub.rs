// check-pass

#![feature(crate_visibility_modifier)]

#![allow(unused)]
#![warn(unreachable_pub)]

mod private_mod {
    // non-leaked `pub` items in private module should be linted
    pub use std::fmt; // { dg-warning "" "" { target *-*-* } }
    pub use std::env::{Args}; // braced-use has different item spans than unbraced
// { dg-warning "" "" { target *-*-* } .-1 }

    pub struct Hydrogen { // { dg-warning "" "" { target *-*-* } }
        // `pub` struct fields, too
        pub neutrons: usize, // { dg-warning "" "" { target *-*-* } }
        // (... but not more-restricted fields)
        crate electrons: usize
    }
    impl Hydrogen {
        // impls, too
        pub fn count_neutrons(&self) -> usize { self.neutrons } // { dg-warning "" "" { target *-*-* } }
        crate fn count_electrons(&self) -> usize { self.electrons }
    }

    pub enum Helium {} // { dg-warning "" "" { target *-*-* } }
    pub union Lithium { c1: usize, c2: u8 } // { dg-warning "" "" { target *-*-* } }
    pub fn beryllium() {} // { dg-warning "" "" { target *-*-* } }
    pub trait Boron {} // { dg-warning "" "" { target *-*-* } }
    pub const CARBON: usize = 1; // { dg-warning "" "" { target *-*-* } }
    pub static NITROGEN: usize = 2; // { dg-warning "" "" { target *-*-* } }
    pub type Oxygen = bool; // { dg-warning "" "" { target *-*-* } }

    macro_rules! define_empty_struct_with_visibility {
        ($visibility: vis, $name: ident) => { $visibility struct $name {} }
// { dg-warning "" "" { target *-*-* } .-1 }
    }
    define_empty_struct_with_visibility!(pub, Fluorine);

    extern {
        pub fn catalyze() -> bool; // { dg-warning "" "" { target *-*-* } }
    }

    // items leaked through signatures (see `get_neon` below) are OK
    pub struct Neon {}

    // crate-visible items are OK
    crate struct Sodium {}
}

pub mod public_mod {
    // module is public: these are OK, too
    pub struct Magnesium {}
    crate struct Aluminum {}
}

pub fn get_neon() -> private_mod::Neon {
    private_mod::Neon {}
}

fn main() {
    let _ = get_neon();
}

