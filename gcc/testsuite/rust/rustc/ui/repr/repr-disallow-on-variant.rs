struct Test;

enum Foo {
    #[repr(u8)]
// { dg-error ".E0517." "" { target *-*-* } .-1 }
    Variant,
}

fn main() {}

