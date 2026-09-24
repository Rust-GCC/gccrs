trait Foo {
    const BAR: u32;
}

struct SignedBar;

impl Foo for SignedBar {
    const BAR: i32 = -1;
// { dg-error ".E0326." "" { target *-*-* } .-1 }
}

fn main() {}

