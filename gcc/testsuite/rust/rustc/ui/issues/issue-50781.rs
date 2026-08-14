#![deny(where_clauses_object_safety)]

trait Trait {}

trait X {
    fn foo(&self) where Self: Trait; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
}

impl X for () {
    fn foo(&self) {}
}

impl Trait for dyn X {}

pub fn main() {
    // Check that this does not segfault.
    <dyn X as X>::foo(&());
}

