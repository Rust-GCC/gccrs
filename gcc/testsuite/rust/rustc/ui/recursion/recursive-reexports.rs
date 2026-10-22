// aux-build:recursive_reexports.rs

extern crate recursive_reexports;

fn f() -> recursive_reexports::S {} // { dg-error ".E0412." "" { target *-*-* } }

fn main() {}

