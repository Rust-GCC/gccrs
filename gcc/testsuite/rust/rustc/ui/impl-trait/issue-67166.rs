// Regression test for #67166

#![feature(impl_trait_in_bindings)]
#![allow(incomplete_features)]

pub fn run() {
    let _foo: Box<impl Copy + '_> = Box::new(());
// { dg-error ".E0106." "" { target *-*-* } .-1 }
}

fn main() {}

