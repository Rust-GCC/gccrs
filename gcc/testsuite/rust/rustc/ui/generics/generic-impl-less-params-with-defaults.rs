use std::marker;

struct Foo<A, B, C = (A, B)>(
    marker::PhantomData<(A,B,C)>);

impl<A, B, C> Foo<A, B, C> {
    fn new() -> Foo<A, B, C> {Foo(marker::PhantomData)}
}

fn main() {
    Foo::<isize>::new();
// { dg-error ".E0107." "" { target *-*-* } .-1 }
}

