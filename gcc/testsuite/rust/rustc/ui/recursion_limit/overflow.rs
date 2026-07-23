// Test the parse error for an overflowing recursion_limit

#![recursion_limit = "999999999999999999999999"]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

