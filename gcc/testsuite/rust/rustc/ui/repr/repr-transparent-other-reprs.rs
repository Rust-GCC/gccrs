#![feature(repr_align)]

// See also repr-transparent.rs

#[repr(transparent, C)] // { dg-error ".E0692." "" { target *-*-* } }
struct TransparentPlusC {
    ptr: *const u8
}

#[repr(transparent, packed)] // { dg-error ".E0692." "" { target *-*-* } }
struct TransparentPlusPacked(*const u8);

#[repr(transparent, align(2))] // { dg-error ".E0692." "" { target *-*-* } }
struct TransparentPlusAlign(u8);

#[repr(transparent)] // { dg-error ".E0692." "" { target *-*-* } }
#[repr(C)]
struct SeparateAttributes(*mut u8);

fn main() {}

