// aux-build:namespaced_enums.rs
extern crate namespaced_enums;

mod m {
    pub use namespaced_enums::Foo::*;
}

pub fn main() {
    use namespaced_enums::Foo::*;

    foo(); // { dg-error ".E0425." "" { target *-*-* } }
    m::foo(); // { dg-error ".E0425." "" { target *-*-* } }
    bar(); // { dg-error ".E0425." "" { target *-*-* } }
    m::bar(); // { dg-error ".E0425." "" { target *-*-* } }
}

