#[linkage = "external"]
static foo: isize = 0;
// { dg-error ".E0658." "" { target *-*-* } .-2 }

fn main() {}

