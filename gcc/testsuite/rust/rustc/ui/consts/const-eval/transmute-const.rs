use std::mem;

static FOO: bool = unsafe { mem::transmute(3u8) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

fn main() {}

