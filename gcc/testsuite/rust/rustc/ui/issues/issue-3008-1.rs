enum Foo {
    Foo_(Bar)
}

enum Bar {
// { dg-error ".E0072." "" { target *-*-* } .-1 }
    BarNone,
    BarSome(Bar)
}

fn main() {
}

