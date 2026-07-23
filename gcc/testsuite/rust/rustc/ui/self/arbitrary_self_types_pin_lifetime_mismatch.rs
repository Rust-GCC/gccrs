use std::pin::Pin;

struct Foo;

impl Foo {
    fn a(self: Pin<&Foo>, f: &Foo) -> &Foo { f } // { dg-error ".E0623." "" { target *-*-* } }

    fn c(self: Pin<&Self>, f: &Foo, g: &Foo) -> (Pin<&Foo>, &Foo) { (self, f) } // { dg-error ".E0623." "" { target *-*-* } }
}

type Alias<T> = Pin<T>;
impl Foo {
    fn bar<'a>(self: Alias<&Self>, arg: &'a ()) -> &() { arg } // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {}

