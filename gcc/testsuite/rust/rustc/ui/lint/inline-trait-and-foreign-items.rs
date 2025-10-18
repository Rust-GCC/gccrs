#![feature(extern_types)]
#![feature(type_alias_impl_trait)]

#![warn(unused_attributes)]

trait Trait {
    #[inline] // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    const X: u32;

    #[inline] // { dg-error ".E0518." "" { target *-*-* } }
    type T;

    type U;
}

impl Trait for () {
    #[inline] // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    const X: u32 = 0;

    #[inline] // { dg-error ".E0518." "" { target *-*-* } }
    type T = Self;

    #[inline] // { dg-error ".E0518." "" { target *-*-* } }
    type U = impl Trait; // { dg-error "" "" { target *-*-* } }
}

extern {
    #[inline] // { dg-error ".E0518." "" { target *-*-* } }
    static X: u32;

    #[inline] // { dg-error ".E0518." "" { target *-*-* } }
    type T;
}

fn main() {}

