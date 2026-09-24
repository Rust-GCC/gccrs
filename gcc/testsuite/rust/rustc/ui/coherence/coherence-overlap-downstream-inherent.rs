// Tests that we consider `T: Sugar + Fruit` to be ambiguous, even
// though no impls are found.

struct Sweet<X>(X);
pub trait Sugar {}
pub trait Fruit {}
impl<T:Sugar> Sweet<T> { fn dummy(&self) { } }
// { dg-error ".E0592." "" { target *-*-* } .-1 }
impl<T:Fruit> Sweet<T> { fn dummy(&self) { } }

trait Bar<X> {}
struct A<T, X>(T, X);
impl<X, T> A<T, X> where T: Bar<X> { fn f(&self) {} }
// { dg-error ".E0592." "" { target *-*-* } .-1 }
impl<X> A<i32, X> { fn f(&self) {} }

fn main() {}

