#![allow(incomplete_features)]
#![feature(impl_trait_in_bindings)]

fn foo() {
    let _ : impl Copy;
// { dg-error ".E0720." "" { target *-*-* } .-1 }
}

fn main() {}

