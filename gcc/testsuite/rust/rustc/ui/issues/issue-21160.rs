struct Bar;

impl Bar {
    fn hash<T>(&self, _: T) {}
}

#[derive(Hash)]
struct Foo(Bar);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {}

