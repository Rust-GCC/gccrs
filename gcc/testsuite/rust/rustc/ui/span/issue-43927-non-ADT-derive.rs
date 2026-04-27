#![allow(dead_code)]

#![derive(Debug, PartialEq, Eq)] // should be an outer attribute!
// { dg-error ".E0774." "" { target *-*-* } .-1 }
// { dg-error ".E0774." "" { target *-*-* } .-2 }
// { dg-error ".E0774." "" { target *-*-* } .-3 }
// { dg-error ".E0774." "" { target *-*-* } .-4 }
// { dg-error ".E0774." "" { target *-*-* } .-5 }
// { dg-error ".E0774." "" { target *-*-* } .-6 }
// { dg-error ".E0774." "" { target *-*-* } .-7 }
struct DerivedOn;

fn main() {}

