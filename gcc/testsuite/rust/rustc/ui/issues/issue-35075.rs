struct Bar<T> {
    inner: Foo<T> // { dg-error ".E0412." "" { target *-*-* } }
}

enum Baz<T> {
    Foo(Foo<T>) // { dg-error ".E0412." "" { target *-*-* } }
}

fn main() {}

