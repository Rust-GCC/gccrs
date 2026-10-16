// Issue #62863
// ignore-tidy-cr

// Note: if you see ^M in this file, that's how your editor renders literal `\r`

/// This doc comment contains three isolated `\r` symbols
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
fn main() {}

