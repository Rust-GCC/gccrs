#![test_runner(main)] // { dg-error ".E0658." "" { target *-*-* } }

#[test_case] // { dg-error ".E0658." "" { target *-*-* } }
fn f() {}

fn main() {}

