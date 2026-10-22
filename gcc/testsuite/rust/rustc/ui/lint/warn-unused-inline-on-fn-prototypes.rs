#![deny(unused_attributes)]

trait Trait {
    #[inline] // { dg-error "" "" { target *-*-* } }
    fn foo();
}

extern {
    #[inline] // { dg-error "" "" { target *-*-* } }
    fn foo();
}

fn main() {}

