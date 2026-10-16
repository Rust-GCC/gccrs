trait Foo {
    fn foo(x: u16);
    fn bar(&mut self, bar: &mut Bar);
}

struct Bar;

impl Foo for Bar {
    fn foo(x: i16) { } // { dg-error ".E0053." "" { target *-*-* } }
    fn bar(&mut self, bar: &Bar) { } // { dg-error ".E0053." "" { target *-*-* } }
}

fn main() {
}

