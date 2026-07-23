fn foo() { }

//! Misplaced comment...
// { dg-error ".E0753." "" { target *-*-* } .-1 }
// { dg-note ".E0753." "" { target *-*-* } .-2 }

fn main() { }

