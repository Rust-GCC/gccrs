type Foo = [u8; 256];

impl Copy for Foo { }
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }

#[derive(Copy, Clone)]
struct Bar;

impl Copy for &'static mut Bar { }
// { dg-error ".E0206." "" { target *-*-* } .-1 }

fn main() {
}

