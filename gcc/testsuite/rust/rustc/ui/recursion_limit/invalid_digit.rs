// Test the parse error for an invalid digit in recursion_limit

#![recursion_limit = "-100"] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

