#![feature(crate_in_paths)]

struct S;

pub mod m {
    fn f() {
        let s = ::m::crate::S; // { dg-error ".E0433." "" { target *-*-* } }
        let s1 = ::crate::S; // { dg-error ".E0433." "" { target *-*-* } }
        let s2 = crate::S; // no error
    }
}

fn main() {}

