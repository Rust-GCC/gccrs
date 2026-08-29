// Regression test for #47511: anonymous lifetimes can appear
// unconstrained in a return type, but only if they appear just once
// in the input, as the input to a projection.

fn f(_: X) -> X {
// { dg-error ".E0581." "" { target *-*-* } .-1 }
    unimplemented!()
}

fn g<'a>(_: X<'a>) -> X<'a> {
// { dg-error ".E0581." "" { target *-*-* } .-1 }
    unimplemented!()
}

type X<'a> = <&'a () as Trait>::Value;

trait Trait {
    type Value;
}

impl<'a> Trait for &'a () {
    type Value = ();
}

fn main() {}

