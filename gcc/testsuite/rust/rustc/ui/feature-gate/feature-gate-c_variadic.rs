#![crate_type="lib"]

pub unsafe extern "C" fn test(_: i32, ap: ...) { }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

