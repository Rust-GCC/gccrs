trait Foo {
    fn foo(_: fn(u8) -> ());
    fn bar(_: Option<u8>);
    fn baz(_: (u8, u16));
    fn qux() -> u8;
}

struct Bar;

impl Foo for Bar {
    fn foo(_: fn(u16) -> ()) {}
// { dg-error ".E0053." "" { target *-*-* } .-1 }
    fn bar(_: Option<u16>) {}
// { dg-error ".E0053." "" { target *-*-* } .-1 }
    fn baz(_: (u16, u16)) {}
// { dg-error ".E0053." "" { target *-*-* } .-1 }
    fn qux() -> u16 { 5u16 }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
}

fn main() {}

