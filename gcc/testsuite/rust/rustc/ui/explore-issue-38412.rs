// aux-build:pub-and-stability.rs

#![feature(unused_feature)]

// A big point of this test is that we *declare* `unstable_declared`,
// but do *not* declare `unstable_undeclared`. This way we can check
// that the compiler is letting in uses of declared feature-gated
// stuff but still rejecting uses of undeclared feature-gated stuff.
#![feature(unstable_declared)]

extern crate pub_and_stability;
use pub_and_stability::{Record, Trait, Tuple};

fn main() {
    // Okay
    let Record { .. } = Record::new();

    // Okay
    let Record { a_stable_pub: _, a_unstable_declared_pub: _, .. } = Record::new();

    let Record { a_stable_pub: _, a_unstable_declared_pub: _, a_unstable_undeclared_pub: _, .. } =
        Record::new();
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    let r = Record::new();
    let t = Tuple::new();

    r.a_stable_pub;
    r.a_unstable_declared_pub;
    r.a_unstable_undeclared_pub; // { dg-error ".E0658." "" { target *-*-* } }
    r.b_crate;                   // { dg-error ".E0616." "" { target *-*-* } }
    r.c_mod;                     // { dg-error ".E0616." "" { target *-*-* } }
    r.d_priv;                    // { dg-error ".E0616." "" { target *-*-* } }

    t.0;
    t.1;
    t.2;                         // { dg-error ".E0658." "" { target *-*-* } }
    t.3;                         // { dg-error ".E0616." "" { target *-*-* } }
    t.4;                         // { dg-error ".E0616." "" { target *-*-* } }
    t.5;                         // { dg-error ".E0616." "" { target *-*-* } }

    r.stable_trait_method();
    r.unstable_declared_trait_method();
    r.unstable_undeclared_trait_method(); // { dg-error ".E0658." "" { target *-*-* } }

    r.stable();
    r.unstable_declared();
    r.unstable_undeclared();              // { dg-error ".E0658." "" { target *-*-* } }

    r.pub_crate();                        // { dg-error ".E0624." "" { target *-*-* } }
    r.pub_mod();                          // { dg-error ".E0624." "" { target *-*-* } }
    r.private();                          // { dg-error ".E0624." "" { target *-*-* } }

    let t = Tuple::new();
    t.stable_trait_method();
    t.unstable_declared_trait_method();
    t.unstable_undeclared_trait_method(); // { dg-error ".E0658." "" { target *-*-* } }

    t.stable();
    t.unstable_declared();
    t.unstable_undeclared();              // { dg-error ".E0658." "" { target *-*-* } }

    t.pub_crate();                        // { dg-error ".E0624." "" { target *-*-* } }
    t.pub_mod();                          // { dg-error ".E0624." "" { target *-*-* } }
    t.private();                          // { dg-error ".E0624." "" { target *-*-* } }
}

