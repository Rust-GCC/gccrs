#![feature(exclusive_range_pattern)]

fn main() {}

#[cfg(FALSE)]
fn foo() {
    if let ..=5 = 0 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    if let ...5 = 0 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
    if let ..5 = 0 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    if let 5.. = 0 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    if let 5..= = 0 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
    if let 5... = 0 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
}

