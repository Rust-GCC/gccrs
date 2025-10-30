#![allow(non_snake_case)]

use std::pin::Pin;

struct Struct<'a> { data: &'a u32 }

impl<'a> Struct<'a> {
    // Test using `&self` sugar:

    fn ref_self(&self, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    // Test using `&Self` explicitly:

    fn ref_Self(self: &Self, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_ref_Self(self: Box<&Self>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn pin_ref_Self(self: Pin<&Self>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_box_ref_Self(self: Box<Box<&Self>>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_pin_Self(self: Box<Pin<&Self>>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }
}

fn main() { }

