mod m {
    pub enum E {
        Fn(u8),
        Struct {
            s: u8,
        },
        Unit,
    }

    pub mod n {
        pub(in m) enum Z {
            Fn(u8),
            Struct {
                s: u8,
            },
            Unit,
        }
    }

    use m::n::Z; // OK, only the type is imported

    fn f() {
        n::Z;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
        Z;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
        let _: Z = Z::Fn;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        let _: Z = Z::Struct;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
        let _ = Z::Unit();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
        let _ = Z::Unit {};
        // This is ok, it is equivalent to not having braces
    }
}

use m::E; // OK, only the type is imported

fn main() {
    let _: E = m::E;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let _: E = m::E::Fn;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let _: E = m::E::Struct;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let _: E = m::E::Unit();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
    let _: E = E;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let _: E = E::Fn;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let _: E = E::Struct;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let _: E = E::Unit();
// { dg-error ".E0618." "" { target *-*-* } .-1 }
    let _: Z = m::n::Z;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
// { dg-error ".E0603." "" { target *-*-* } .-2 }
// { dg-error ".E0603." "" { target *-*-* } .-3 }
    let _: Z = m::n::Z::Fn;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
// { dg-error ".E0603." "" { target *-*-* } .-2 }
    let _: Z = m::n::Z::Struct;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
// { dg-error ".E0603." "" { target *-*-* } .-2 }
// { dg-error ".E0603." "" { target *-*-* } .-3 }
    let _: Z = m::n::Z::Unit {};
// { dg-error ".E0603." "" { target *-*-* } .-1 }
// { dg-error ".E0603." "" { target *-*-* } .-2 }
}

