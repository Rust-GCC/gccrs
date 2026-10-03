// edition:2018

use std::pin::Pin;

struct Foo;

impl Foo {
    async fn a(self: Pin<&Foo>, f: &Foo) -> &Foo { f }
// { dg-error ".E0623." "" { target *-*-* } .-1 }

    async fn c(self: Pin<&Self>, f: &Foo, g: &Foo) -> (Pin<&Foo>, &Foo) { (self, f) }
// { dg-error ".E0623." "" { target *-*-* } .-1 }
}

type Alias<T> = Pin<T>;
impl Foo {
    async fn bar<'a>(self: Alias<&Self>, arg: &'a ()) -> &() { arg } // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {}

