#![allow(warnings)]
#![feature(in_band_lifetimes)]

trait Get {
    fn foo(&self, x: &'a u32, y: &u32) -> &'a u32;
}

impl Get for i32 {
    fn foo(&self, x: &u32, y: &'a u32) -> &'a u32 { // { dg-error "" "" { target *-*-* } }
        x // { dg-error ".E0623." "" { target *-*-* } }
    }
}

fn main() {}

