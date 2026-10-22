// Test that `dyn ?Sized` (i.e., a trait object with only a maybe buond) is not allowed.

type _0 = dyn ?Sized;
// { dg-error ".E0224." "" { target *-*-* } .-1 }
// { dg-error ".E0224." "" { target *-*-* } .-2 }

fn main() {}

