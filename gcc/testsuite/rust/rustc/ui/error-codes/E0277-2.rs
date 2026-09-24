struct Foo {
    bar: Bar
}

struct Bar {
    baz: Baz
}

struct Baz {
    x: *const u8
}

fn is_send<T: Send>() { }

fn main() {
    is_send::<Foo>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

