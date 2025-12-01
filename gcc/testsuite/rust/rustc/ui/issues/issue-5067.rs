#![allow(unused_macros)]

// Tests that repetition matchers cannot match the empty token tree (since that would be
// ambiguous).

// edition:2018

macro_rules! foo {
    ( $()* ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $()+ ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $()? ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $(),* ) => {}; // PASS
    ( $(),+ ) => {}; // PASS
    // `?` cannot have a separator...
    ( [$()*] ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( [$()+] ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( [$()?] ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( [$(),*] ) => {}; // PASS
    ( [$(),+] ) => {}; // PASS
    // `?` cannot have a separator...
    ( $($()* $(),* $(a)* $(a),* )* ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $($()* $(),* $(a)* $(a),* )+ ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $($()* $(),* $(a)* $(a),* )? ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $($()? $(),* $(a)? $(a),* )* ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $($()? $(),* $(a)? $(a),* )+ ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $($()? $(),* $(a)? $(a),* )? ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $(a     $(),* $(a)* $(a),* )* ) => {}; // PASS
    ( $($(a)+ $(),* $(a)* $(a),* )+ ) => {}; // PASS
    ( $($(a)+ $(),* $(a)* $(a),* )? ) => {}; // PASS

    ( $(a     $(),* $(a)? $(a),* )* ) => {}; // PASS
    ( $($(a)+ $(),* $(a)? $(a),* )+ ) => {}; // PASS
    ( $($(a)+ $(),* $(a)? $(a),* )? ) => {}; // PASS

    ( $(a $()+)* ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $(a $()*)+ ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $(a $()+)? ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
    ( $(a $()?)+ ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
}

// Original Issue

macro_rules! make_vec {
    (a $e1:expr $($(, a $e2:expr)*)*) => ([$e1 $($(, $e2)*)*]);
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    let _ = make_vec![a 1, a 2, a 3];
}

// Minified Issue

macro_rules! m {
    ( $()* ) => {};
// { dg-error "" "" { target *-*-* } .-1 }
}

m!();

