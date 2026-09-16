#![feature(untagged_unions)]

union U {
    a: str,
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    b: u8,
}

union W {
    a: u8,
    b: str,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

