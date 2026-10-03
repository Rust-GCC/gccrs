use std::marker;

struct Foo<T> { foo: Option<Option<Foo<T>>>, marker: marker::PhantomData<T> }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

impl<T> Foo<T> { fn bar(&self) {} }

fn main() {}

