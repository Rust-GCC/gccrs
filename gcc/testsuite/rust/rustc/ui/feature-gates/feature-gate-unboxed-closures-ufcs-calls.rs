#![allow(dead_code)]

fn foo<F: Fn()>(mut f: F) {
    Fn::call(&f, ()); // { dg-error ".E0658." "" { target *-*-* } }
    FnMut::call_mut(&mut f, ()); // { dg-error ".E0658." "" { target *-*-* } }
    FnOnce::call_once(f, ()); // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

