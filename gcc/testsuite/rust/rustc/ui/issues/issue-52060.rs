// Regression test for https://github.com/rust-lang/rust/issues/52060
// The compiler shouldn't ICE in this case
static A: &'static [u32] = &[1];
static B: [u32; 1] = [0; A.len()];
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }

fn main() {}

