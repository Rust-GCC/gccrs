#![feature(decl_macro)]

mod m {
    macro mac() {}
}

fn main() {
    m::mac!(); // { dg-error ".E0603." "" { target *-*-* } }
}

