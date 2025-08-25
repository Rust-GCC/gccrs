// compile-flags: -F deprecated

#[allow(deprecated)] // { dg-error ".E0453." "" { target *-*-* } }
// { dg-error ".E0453." "" { target *-*-* } .-2 }
// { dg-error ".E0453." "" { target *-*-* } .-3 }
fn main() {
}

