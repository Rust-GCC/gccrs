// Test sized-ness checking in substitution in impls.

// impl - struct

struct S5<Y>(Y);

impl<X: ?Sized> S5<X> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() { }

