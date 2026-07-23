#![allow(unused)]

extern crate core;
pub use core as reexported_core; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }

mod foo1 {
    extern crate core;
}

mod foo2 {
    use foo1::core; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    pub mod bar {
        extern crate core;
    }
}

mod baz {
    pub use foo2::bar::core; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
}

fn main() {}

