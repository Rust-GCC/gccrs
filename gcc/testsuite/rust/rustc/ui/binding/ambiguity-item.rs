// Identifier pattern referring to an ambiguity item is an error (issue #46079).

mod m {
    pub fn f() {}
}
use m::*;

mod n {
    pub fn f() {}
}
use n::*; // OK, no conflict with `use m::*;`

fn main() {
    let v = f; // { dg-error ".E0659." "" { target *-*-* } }
    match v {
        f => {} // { dg-error ".E0659." "" { target *-*-* } }
        mut f => {} // OK, unambiguously a fresh binding due to `mut`
    }
}

