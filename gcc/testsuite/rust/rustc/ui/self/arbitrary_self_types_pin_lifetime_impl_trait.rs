use std::pin::Pin;

struct Foo;

impl Foo {
    fn f(self: Pin<&Self>) -> impl Clone { self } // { dg-error ".E0759." "" { target *-*-* } }
}

fn main() {
    { Pin::new(&Foo).f() };
}

