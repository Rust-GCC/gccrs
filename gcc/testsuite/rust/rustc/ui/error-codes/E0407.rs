trait Foo {
    fn a();
}

struct Bar;

impl Foo for Bar {
    fn a() {}
    fn b() {}
// { dg-error ".E0407." "" { target *-*-* } .-1 }
}

fn main() {
}

