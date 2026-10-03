#![feature(generic_associated_types)]
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

struct X;

impl X {
    type Y;
// { dg-error ".E0202." "" { target *-*-* } .-1 }
// { dg-error ".E0202." "" { target *-*-* } .-2 }
    type Z: Ord;
// { dg-error ".E0202." "" { target *-*-* } .-1 }
// { dg-error ".E0202." "" { target *-*-* } .-2 }
// { dg-error ".E0202." "" { target *-*-* } .-3 }
    type W: Ord where Self: Eq;
// { dg-error ".E0202." "" { target *-*-* } .-1 }
// { dg-error ".E0202." "" { target *-*-* } .-2 }
// { dg-error ".E0202." "" { target *-*-* } .-3 }
    type W where Self: Eq;
// { dg-error ".E0202." "" { target *-*-* } .-1 }
// { dg-error ".E0202." "" { target *-*-* } .-2 }
}

