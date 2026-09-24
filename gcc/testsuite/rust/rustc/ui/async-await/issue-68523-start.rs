// edition:2018

#![feature(start)]

#[start]
pub async fn start(_: isize, _: *const *const u8) -> isize {
// { dg-error ".E0752." "" { target *-*-* } .-1 }
    0
}

