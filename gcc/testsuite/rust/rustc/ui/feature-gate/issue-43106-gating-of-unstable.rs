// Testing gating of `#[unstable]` in "weird" places.
//
// This file sits on its own because these signal errors, making
// this test incompatible with the "warnings only" nature of
// issue-43106-gating-of-builtin-attrs.rs

#![unstable()]
// { dg-error ".E0734." "" { target *-*-* } .-1 }

#[unstable()]
// { dg-error ".E0734." "" { target *-*-* } .-1 }
mod unstable {
    mod inner { #![unstable()] }
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[unstable()] fn f() { }
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[unstable()] struct S;
// { dg-error ".E0734." "" { target *-*-* } .-1 }
// { dg-error ".E0734." "" { target *-*-* } .-2 }

    #[unstable()] type T = S;
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[unstable()] impl S { }
// { dg-error ".E0734." "" { target *-*-* } .-1 }
}

fn main() {}

