// Check that we do not allow casts or coercions
// to object unsafe trait objects by ref

#![feature(object_safe_for_dispatch)]

trait Trait: Sized {}

struct S;

impl Trait for S {}

fn takes_trait(t: &dyn Trait) {}

fn main() {
    &S as &dyn Trait; // { dg-error ".E0038." "" { target *-*-* } }
    let t: &dyn Trait = &S; // { dg-error ".E0038." "" { target *-*-* } }
    takes_trait(&S); // { dg-error ".E0038." "" { target *-*-* } }
}

