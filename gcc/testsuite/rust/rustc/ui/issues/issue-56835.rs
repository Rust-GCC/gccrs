pub struct Foo {}

impl Foo {
    fn bar(Self(foo): Self) {}
// { dg-error ".E0164." "" { target *-*-* } .-1 }
// { dg-error ".E0164." "" { target *-*-* } .-2 }
}

fn main() {}

