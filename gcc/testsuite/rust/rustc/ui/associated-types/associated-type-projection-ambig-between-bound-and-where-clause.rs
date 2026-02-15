// Test equality constraints in a where clause where the type being
// equated appears in a supertrait.

pub trait Vehicle {
    type Color;

    fn go(&self) {  }
}

pub trait Box {
    type Color;

    fn mail(&self) {  }
}

fn a<C:Vehicle+Box>(_: C::Color) {
// { dg-error ".E0221." "" { target *-*-* } .-1 }
}

fn b<C>(_: C::Color) where C : Vehicle+Box {
// { dg-error ".E0221." "" { target *-*-* } .-1 }
}

fn c<C>(_: C::Color) where C : Vehicle, C : Box {
// { dg-error ".E0221." "" { target *-*-* } .-1 }
}

struct D<X>;
impl<X> D<X> where X : Vehicle {
    fn d(&self, _: X::Color) where X : Box { }
// { dg-error ".E0221." "" { target *-*-* } .-1 }
}

trait E<X:Vehicle> {
    fn e(&self, _: X::Color) where X : Box;
// { dg-error ".E0221." "" { target *-*-* } .-1 }

    fn f(&self, _: X::Color) where X : Box { }
// { dg-error ".E0221." "" { target *-*-* } .-1 }
}

pub fn main() { }

