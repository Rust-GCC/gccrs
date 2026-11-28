enum Foo {
    Bar(i32),
    Baz
}

struct S;

fn main() {
    match Foo::Baz {
        Foo::Bar => {}
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        _ => {}
    }

    match S {
        S(()) => {}
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
}

