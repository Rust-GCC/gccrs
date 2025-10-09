// edition:2018
#![allow(non_camel_case_types)]

mod outer_mod {
    pub mod await { // { dg-error "" "" { target *-*-* } }
        pub struct await; // { dg-error "" "" { target *-*-* } }
    }
}
use self::outer_mod::await::await; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

macro_rules! await { () => {}; } // { dg-error "" "" { target *-*-* } }

fn main() {
    await!(); // { dg-error "" "" { target *-*-* } }
}

