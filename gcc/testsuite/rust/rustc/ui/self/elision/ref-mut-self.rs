#![allow(non_snake_case)]

use std::pin::Pin;

struct Struct { }

impl Struct {
    // Test using `&mut self` sugar:

    fn ref_self(&mut self, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    // Test using `&mut Self` explicitly:

    fn ref_Self(self: &mut Self, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_ref_Self(self: Box<&mut Self>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn pin_ref_Self(self: Pin<&mut Self>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_box_ref_Self(self: Box<Box<&mut Self>>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_pin_ref_Self(self: Box<Pin<&mut Self>>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }
}

fn main() { }

