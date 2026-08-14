mod m1 {
    pub use ::E::V; // { dg-error "" "" { target *-*-* } }
}

mod m2 {
    pub use ::E::{V}; // { dg-error "" "" { target *-*-* } }
}

mod m3 {
    pub use ::E::V::{self}; // { dg-error "" "" { target *-*-* } }
}

mod m4 {
    pub use ::E::*; // { dg-error "" "" { target *-*-* } }
}

enum E { V }

fn main() {}

