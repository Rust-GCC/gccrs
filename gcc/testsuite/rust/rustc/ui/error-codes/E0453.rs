#![forbid(non_snake_case)]

#[allow(non_snake_case)]
// { dg-error ".E0453." "" { target *-*-* } .-1 }
// { dg-error ".E0453." "" { target *-*-* } .-2 }
// { dg-error ".E0453." "" { target *-*-* } .-3 }
fn main() {
}

