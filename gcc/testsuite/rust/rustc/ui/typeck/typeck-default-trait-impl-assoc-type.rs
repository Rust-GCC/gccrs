// run-rustfix
// Test that we do not consider associated types to be sendable without
// some applicable trait bound (and we don't ICE).
#![allow(dead_code)]

trait Trait {
    type AssocType;
    fn dummy(&self) { }
}
fn bar<T:Trait+Send>() {
    is_send::<T::AssocType>(); // { dg-error ".E0277." "" { target *-*-* } }
}

fn is_send<T:Send>() {
}

fn main() { }

