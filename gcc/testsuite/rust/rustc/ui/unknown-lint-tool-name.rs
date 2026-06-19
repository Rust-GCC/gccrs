#![deny(foo::bar)] // { dg-error ".E0710." "" { target *-*-* } }
// { dg-error ".E0710." "" { target *-*-* } .-2 }
// { dg-error ".E0710." "" { target *-*-* } .-3 }

#[allow(foo::bar)] // { dg-error ".E0710." "" { target *-*-* } }
// { dg-error ".E0710." "" { target *-*-* } .-2 }
// { dg-error ".E0710." "" { target *-*-* } .-3 }
fn main() {}

