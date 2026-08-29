// check-pass
#![warn(const_err)]

#![crate_type = "lib"]

pub const Z: u32 = 0 - 1;
// { dg-warning "" "" { target *-*-* } .-1 }

pub type Foo = [i32; 0 - 1];

