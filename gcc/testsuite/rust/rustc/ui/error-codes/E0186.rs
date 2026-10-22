trait Foo {
    fn foo(&self); // { dg-error "" "" { target *-*-* } }
}

struct Bar;

impl Foo for Bar {
    fn foo() {} // { dg-error ".E0186." "" { target *-*-* } }
// { dg-error ".E0186." "" { target *-*-* } .-1 }
}

fn main() {
}

