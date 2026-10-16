// Test that we can't declare a const fn in an impl -- right now it's
// just not allowed at all, though eventually it'd make sense to allow
// it if the trait fn is const (but right now no trait fns can be
// const).

#![feature(const_fn)]

trait Foo {
    fn f() -> u32;
}

impl Foo for u32 {
    const fn f() -> u32 {
// { dg-error ".E0379." "" { target *-*-* } .-1 }
        22
    }
}

fn main() {}

