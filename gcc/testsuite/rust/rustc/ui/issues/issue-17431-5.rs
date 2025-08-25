use std::marker;

struct Foo { foo: Bar<Foo> }

struct Bar<T> { x: Bar<Foo> , marker: marker::PhantomData<T> }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

impl Foo { fn foo(&self) {} }

fn main() {
}

