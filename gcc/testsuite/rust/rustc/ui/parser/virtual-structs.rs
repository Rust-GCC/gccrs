// Test diagnostics for the removed struct inheritance feature.

virtual struct SuperStruct {
// { dg-error "" "" { target *-*-* } .-1 }
    f1: isize,
}

struct Struct : SuperStruct;

pub fn main() {}

