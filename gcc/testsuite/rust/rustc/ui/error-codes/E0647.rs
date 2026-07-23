#![no_std]
#![feature(start)]

extern crate std;

#[start]
fn start(_: isize, _: *const *const u8) -> isize where (): Copy { // { dg-error ".E0647." "" { target *-*-* } }
    0
}

