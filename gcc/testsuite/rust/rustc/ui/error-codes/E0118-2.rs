struct Foo;

impl &mut Foo {
// { dg-error ".E0118." "" { target *-*-* } .-1 }
    fn bar(self) {}
}

fn main() {}

