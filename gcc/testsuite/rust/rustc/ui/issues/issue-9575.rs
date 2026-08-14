#![feature(start)]

#[start]
fn start(argc: isize, argv: *const *const u8, crate_map: *const u8) -> isize {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
   0
}

