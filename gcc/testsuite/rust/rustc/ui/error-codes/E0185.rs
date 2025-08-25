trait Foo {
    fn foo();
// { dg-note "" "" { target *-*-* } .-1 }
}

struct Bar;

impl Foo for Bar {
    fn foo(&self) {}
// { dg-error ".E0185." "" { target *-*-* } .-1 }
// { dg-note ".E0185." "" { target *-*-* } .-2 }
}

fn main() {
}

