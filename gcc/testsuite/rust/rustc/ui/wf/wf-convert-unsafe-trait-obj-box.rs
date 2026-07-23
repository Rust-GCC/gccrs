// Check that we do not allow casts or coercions
// to object unsafe trait objects inside a Box

#![feature(object_safe_for_dispatch)]

trait Trait: Sized {}

struct S;

impl Trait for S {}

fn takes_box(t: Box<dyn Trait>) {}

fn main() {
    Box::new(S) as Box<dyn Trait>; // { dg-error ".E0038." "" { target *-*-* } }
    let t_box: Box<dyn Trait> = Box::new(S); // { dg-error ".E0038." "" { target *-*-* } }
    takes_box(Box::new(S)); // { dg-error ".E0038." "" { target *-*-* } }
}

