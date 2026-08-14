#![deny(variant_size_differences)]

enum _En {
    V0(u8),
    VBig([u8; 1024]),   // { dg-error "" "" { target *-*-* } }
}

fn main() {}

