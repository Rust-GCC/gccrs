enum Foo {}
type FooAlias = Foo;

fn main() {
    let u = FooAlias { value: 0 };
// { dg-error ".E0071." "" { target *-*-* } .-1 }
}

