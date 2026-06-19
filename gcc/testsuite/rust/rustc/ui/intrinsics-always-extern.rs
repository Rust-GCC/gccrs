#![feature(intrinsics)]

trait Foo {
    extern "rust-intrinsic" fn foo(&self); // { dg-error "" "" { target *-*-* } }
}

impl Foo for () {
    extern "rust-intrinsic" fn foo(&self) { // { dg-error "" "" { target *-*-* } }
    }
}

extern "rust-intrinsic" fn hello() {// { dg-error "" "" { target *-*-* } }
}

fn main() {
}

