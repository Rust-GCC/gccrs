enum Foo {
    Bar { bar: Bar, id: usize }
}

enum Bar {
    A, B, C, D, E, F
}

fn test(f: Foo) {
    match f {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
// { dg-error ".E0004." "" { target *-*-* } .-2 }
        Foo::Bar { bar: Bar::A, .. } => (),
        Foo::Bar { bar: Bar::B, .. } => (),
    }
}

fn main() {}

