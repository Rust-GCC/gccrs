#![allow(non_snake_case)]

use std::pin::Pin;

struct Struct { }

impl Struct {
    // Test using `&mut Struct` explicitly:

    fn ref_Struct(self: &mut Struct, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_ref_Struct(self: Box<&mut Struct>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn pin_ref_Struct(self: Pin<&mut Struct>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_box_ref_Struct(self: Box<Box<&mut Struct>>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }

    fn box_pin_ref_Struct(self: Box<Pin<&mut Struct>>, f: &u32) -> &u32 {
        f // { dg-error ".E0623." "" { target *-*-* } }
    }
}

fn main() { }

