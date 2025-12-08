// ignore-tidy-linelength

struct Bar;

impl Bar {
    fn bar(&mut self, _: impl Fn()) {}
}

struct Foo {
    thing: Bar,
    number: usize,
}

impl Foo {
    fn foo(&mut self) {
        self.thing.bar(|| {
// { dg-error ".E0502." "" { target *-*-* } .-1 }
            &self.number;
        });
    }
}

fn main() {}

