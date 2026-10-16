#![feature(const_fn)]

trait Foo {
    fn f() -> u32;
    const fn g(); // { dg-error ".E0379." "" { target *-*-* } }
}

impl Foo for u32 {
    const fn f() -> u32 { 22 } // { dg-error ".E0379." "" { target *-*-* } }
    fn g() {}
}

fn main() { }

