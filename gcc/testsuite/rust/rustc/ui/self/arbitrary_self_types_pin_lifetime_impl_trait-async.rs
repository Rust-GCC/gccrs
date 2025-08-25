// edition:2018

use std::pin::Pin;

struct Foo;

impl Foo {
    async fn f(self: Pin<&Self>) -> impl Clone { self }
// { dg-error ".E0759." "" { target *-*-* } .-1 }
}

fn main() {
    { Pin::new(&Foo).f() };
}

