enum Foo {}
struct Bar {}

impl Foo {
    fn foo() {
        let _ = Self;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = Self();
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

impl Bar {
    fn bar() {
        let _ = Self;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = Self();
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

fn main() {}

