enum Thing {
    Foo(u8),
    Bar,
    Baz
}

fn main() {
    let Thing::Foo(y) = Thing::Foo(1);
// { dg-error ".E0005." "" { target *-*-* } .-1 }
}

