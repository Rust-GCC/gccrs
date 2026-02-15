#![forbid(deprecated)]

#[allow(deprecated)]
// { dg-error ".E0453." "" { target *-*-* } .-1 }
// { dg-error ".E0453." "" { target *-*-* } .-2 }
// { dg-error ".E0453." "" { target *-*-* } .-3 }
fn main() {
}

