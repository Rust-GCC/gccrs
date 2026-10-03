#![allow(const_err)] // make sure we cannot allow away the errors tested here

//! Test the "array of int" fast path in validity checking, and in particular whether it
//! points at the right array element.

use std::mem;

#[repr(C)]
union MaybeUninit<T: Copy> {
    uninit: (),
    init: T,
}

const UNINIT_INT_0: [u32; 3] = unsafe {
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
    [
        MaybeUninit { uninit: () }.init,
        1,
        2,
    ]
};
const UNINIT_INT_1: [u32; 3] = unsafe {
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
    mem::transmute(
        [
            0u8,
            0u8,
            0u8,
            0u8,
            1u8,
            MaybeUninit { uninit: () }.init,
            1u8,
            1u8,
            2u8,
            2u8,
            MaybeUninit { uninit: () }.init,
            2u8,
        ]
    )
};
const UNINIT_INT_2: [u32; 3] = unsafe {
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
    mem::transmute(
        [
            0u8,
            0u8,
            0u8,
            0u8,
            1u8,
            1u8,
            1u8,
            1u8,
            2u8,
            2u8,
            2u8,
            MaybeUninit { uninit: () }.init,
        ]
    )
};

fn main() {}

