#![feature(associated_type_defaults)]

// Tests that `Self` is not assumed to implement `Sized` when used as an
// associated type default.

trait Inner<S> {}

trait MyTrait {
    type This = Self;  // { dg-error ".E0277." "" { target *-*-* } }
    fn something<I: Inner<Self::This>>(i: I);
}

fn main() {}

