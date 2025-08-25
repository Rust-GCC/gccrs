#![allow(incomplete_features)]
#![feature(generic_associated_types)]

trait ATy {
    type Item<'a>: 'a;
}

impl<'b> ATy for &'b () {
    type Item<'a> = &'b ();
// { dg-error ".E0477." "" { target *-*-* } .-1 }
}

trait StaticTy {
    type Item<'a>: 'static;
}

impl StaticTy for () {
    type Item<'a> = &'a ();
// { dg-error ".E0477." "" { target *-*-* } .-1 }
}

fn main() {}

