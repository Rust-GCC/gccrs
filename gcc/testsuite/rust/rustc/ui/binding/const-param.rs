// Identifier pattern referring to a const generic parameter is an error (issue #68853).

#![feature(const_generics)] // { dg-warning "" "" { target *-*-* } }

fn check<const N: usize>() {
    match 1 {
        N => {} // { dg-error ".E0158." "" { target *-*-* } }
        _ => {}
    }
}

fn main() {}

