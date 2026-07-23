// aux-build:builtin-attrs.rs

#![feature(decl_macro)] // { dg-error ".E0659." "" { target *-*-* } }

extern crate builtin_attrs;
use builtin_attrs::{test, bench};
use builtin_attrs::*;

#[repr(C)] // { dg-error ".E0659." "" { target *-*-* } }
struct S;
#[cfg_attr(all(), repr(C))] // { dg-error ".E0659." "" { target *-*-* } }
struct SCond;

#[test] // OK, shadowed
fn test() {}

#[bench] // OK, shadowed
fn bench() {}

fn non_macro_expanded_location<#[repr(C)] T>() { // { dg-error ".E0659." "" { target *-*-* } }
    match 0u8 {
        #[repr(C)] // { dg-error ".E0659." "" { target *-*-* } }
        _ => {}
    }
}

fn main() {
    Test;
    Bench;
    NonExistent; // { dg-error ".E0425." "" { target *-*-* } }
}

