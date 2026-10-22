#![allow(bare_trait_objects)]

trait Foo {}

struct Bar<'a> {
    w: &'a Foo + Copy, // { dg-error ".E0178." "" { target *-*-* } }
    x: &'a Foo + 'a, // { dg-error ".E0178." "" { target *-*-* } }
    y: &'a mut Foo + 'a, // { dg-error ".E0178." "" { target *-*-* } }
    z: fn() -> Foo + 'a, // { dg-error ".E0178." "" { target *-*-* } }
}

fn main() {
}

