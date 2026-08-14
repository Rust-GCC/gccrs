// Test syntax checks for `type` keyword.

struct S1 for type;
// { dg-error "" "" { target *-*-* } .-1 }

pub fn main() {
}

