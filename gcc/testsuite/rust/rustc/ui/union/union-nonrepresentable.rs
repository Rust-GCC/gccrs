#![feature(untagged_unions)]

union U { // { dg-error ".E0072." "" { target *-*-* } }
    a: u8,
    b: U,
}

fn main() {}

