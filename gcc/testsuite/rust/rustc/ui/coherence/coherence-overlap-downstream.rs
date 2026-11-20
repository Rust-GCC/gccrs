// Tests that we consider `T: Sugar + Fruit` to be ambiguous, even
// though no impls are found.

pub trait Sugar {}
pub trait Fruit {}
pub trait Sweet {}
impl<T:Sugar> Sweet for T { }
impl<T:Fruit> Sweet for T { }
// { dg-error ".E0119." "" { target *-*-* } .-1 }

pub trait Foo<X> {}
pub trait Bar<X> {}
impl<X, T> Foo<X> for T where T: Bar<X> {}
impl<X> Foo<X> for i32 {}
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() { }

