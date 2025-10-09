fn main() {}

fn syntax() {
    fn foo() = 42; // { dg-error "" "" { target *-*-* } }
    fn bar() -> u8 = 42; // { dg-error "" "" { target *-*-* } }
}

extern {
    fn foo() = 42; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    fn bar() -> u8 = 42; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

trait Foo {
    fn foo() = 42; // { dg-error "" "" { target *-*-* } }
    fn bar() -> u8 = 42; // { dg-error "" "" { target *-*-* } }
}

impl Foo for () {
    fn foo() = 42; // { dg-error "" "" { target *-*-* } }
    fn bar() -> u8 = 42; // { dg-error "" "" { target *-*-* } }
}

