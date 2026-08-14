trait Foo {
    #[derive(Clone)]
// { dg-error ".E0774." "" { target *-*-* } .-1 }
    type Bar;
}

struct Bar;

impl Bar {
    #[derive(Clone)]
// { dg-error ".E0774." "" { target *-*-* } .-1 }
    fn bar(&self) {}
}

fn main() {}

