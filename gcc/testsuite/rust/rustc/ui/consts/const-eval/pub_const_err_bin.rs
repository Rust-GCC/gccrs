// check-pass
#![warn(const_err)]

pub const Z: u32 = 0 - 1;
// { dg-warning "" "" { target *-*-* } .-1 }

pub type Foo = [i32; 0 - 1];

fn main() {}

