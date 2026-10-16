trait Foo {
    fn foo();
}

struct Bar;

impl Foo for Bar {}
// { dg-error ".E0046." "" { target *-*-* } .-1 }

fn main() {
}

