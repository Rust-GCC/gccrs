#![feature(impl_trait_in_bindings)]
#![allow(incomplete_features)]

fn main() {
    const C: impl Copy = 0;
    match C {
        C | // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
}

