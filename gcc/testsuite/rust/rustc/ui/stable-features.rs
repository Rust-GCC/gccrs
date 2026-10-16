// Testing that the stable_features lint catches use of stable
// language and lib features.

#![deny(stable_features)]

#![feature(test_accepted_feature)]
// { dg-error "" "" { target *-*-* } .-1 }

#![feature(rust1)]
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let _foo: Vec<()> = Vec::new();
}

