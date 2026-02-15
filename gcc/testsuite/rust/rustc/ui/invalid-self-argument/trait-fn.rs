struct Foo {}

impl Foo {
    fn c(foo: u32, self) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    fn good(&mut self, foo: u32) {}
}

fn main() { }

