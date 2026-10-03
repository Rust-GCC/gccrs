#![allow(non_camel_case_types)]

struct hello(isize);

fn main() {
    let hello = 0; // { dg-error ".E0530." "" { target *-*-* } }
}

