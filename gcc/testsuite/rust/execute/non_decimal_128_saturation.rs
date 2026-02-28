// { dg-do run }
// { dg-require-effective-target lp64 }
// { dg-options "-O0" }
#![feature(no_core)]
#![no_core]

fn main() -> i32 {
    let hex_val: u128 = 0x1_0000_0000_0000_0000_u128;
    if (hex_val >> 64) as u8 != 1 {
        return 1;
    }

    let bin_val: u128 =
        0b1_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_u128;
    if (bin_val >> 64) as u8 != 1 {
        return 1;
    }

    let oct_val: u128 = 0o2_000_000_000_000_000_000_000_u128;
    if (oct_val >> 64) as u8 != 1 {
        return 1;
    }
    0
}
