struct Foo<A> { inner: A }

trait Bar { fn bar(); }

impl Bar for Foo<i32> {
    fn bar() {
        Self { inner: 1.5f32 }; // { dg-error ".E0308." "" { target *-*-* } }
    }
}

impl<T> Foo<T> {
    fn new<U>(u: U) -> Foo<U> {
        Self {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
            inner: u
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        }
    }
}

fn main() {}

