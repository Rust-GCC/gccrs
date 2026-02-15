#![allow(dead_code)]

fn foo<F: Fn()>(mut f: F) {
    f.call(()); // { dg-error ".E0658." "" { target *-*-* } }
    f.call_mut(()); // { dg-error ".E0658." "" { target *-*-* } }
    f.call_once(()); // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

