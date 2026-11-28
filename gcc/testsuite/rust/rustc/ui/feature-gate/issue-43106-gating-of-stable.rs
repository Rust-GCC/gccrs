// Testing gating of `#[stable]` in "weird" places.
//
// This file sits on its own because these signal errors, making
// this test incompatible with the "warnings only" nature of
// issue-43106-gating-of-builtin-attrs.rs

#![stable()]
// { dg-error ".E0734." "" { target *-*-* } .-1 }

#[stable()]
// { dg-error ".E0734." "" { target *-*-* } .-1 }
mod stable {
    mod inner { #![stable()] }
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[stable()] fn f() { }
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[stable()] struct S;
// { dg-error ".E0734." "" { target *-*-* } .-1 }
// { dg-error ".E0734." "" { target *-*-* } .-2 }

    #[stable()] type T = S;
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[stable()] impl S { }
// { dg-error ".E0734." "" { target *-*-* } .-1 }
}

fn main() {}

