// Evaluation of constants in array-elem count goes through different
// compiler control-flow paths.
//
// This test is checking the count in an array type.

#![allow(unused_imports)]

use std::{i8, i16, i32, i64, isize};
use std::{u8, u16, u32, u64, usize};

const A_I8_T
    : [u32; (i8::MAX as i8 + 1u8) as usize]
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
    = [0; (i8::MAX as usize) + 1];


const A_CHAR_USIZE
    : [u32; 5u8 as char as usize]
    = [0; 5];


const A_BAD_CHAR_USIZE
    : [u32; 5i8 as char as usize]
// { dg-error ".E0604." "" { target *-*-* } .-1 }
    = [0; 5];

fn main() {}

