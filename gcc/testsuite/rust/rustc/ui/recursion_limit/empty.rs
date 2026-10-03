// Test the parse error for an empty recursion_limit

#![recursion_limit = ""] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

