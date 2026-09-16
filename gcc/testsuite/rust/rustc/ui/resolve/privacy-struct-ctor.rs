// aux-build:privacy-struct-ctor.rs

extern crate privacy_struct_ctor as xcrate;

mod m {
    pub struct S(u8);
    pub struct S2 {
        s: u8
    }

    pub mod n {
        pub(in m) struct Z(pub(in m::n) u8);
    }

    use m::n::Z; // OK, only the type is imported

    fn f() {
        n::Z;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
        Z;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    }
}

use m::S; // OK, only the type is imported
use m::S2; // OK, only the type is imported

fn main() {
    m::S;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    let _: S = m::S(2);
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    S;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    m::n::Z;
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    S2;
// { dg-error ".E0423." "" { target *-*-* } .-1 }

    xcrate::m::S;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    xcrate::S;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    xcrate::m::n::Z;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

