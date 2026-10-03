// Regression test for issue 50714, make sure that this isn't a linker error.

#![no_std]
#![feature(start)]

extern crate std;

#[start]
fn start(_: isize, _: *const *const u8) -> isize where fn(&()): Eq { // { dg-error ".E0647." "" { target *-*-* } }
    0
}

