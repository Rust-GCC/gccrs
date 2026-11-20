trait Foo {
    fn foo(x: u16);
    fn bar(&self);
}

struct Bar;

impl Foo for Bar {
    fn foo(x: i16) { }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
    fn bar(&mut self) { }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
}

fn main() {
}

