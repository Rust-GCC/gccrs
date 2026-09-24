// compile-flags: -Zunleash-the-miri-inside-of-you

#![allow(dead_code)]

const TEST: &u8 = &MY_STATIC;
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
// { dg-note ".E0080." "" { target *-*-* } .-3 }

static MY_STATIC: u8 = 4;

fn main() {
}

