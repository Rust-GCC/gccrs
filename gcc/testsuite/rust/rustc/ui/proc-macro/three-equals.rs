// aux-build:three-equals.rs

extern crate three_equals;

use three_equals::three_equals;

fn main() {
    // This one is okay.
    three_equals!(===);

    // Need exactly three equals.
    three_equals!(==); // { dg-error "" "" { target *-*-* } }

    // Need exactly three equals.
    three_equals!(=====); // { dg-error "" "" { target *-*-* } }

    // Only equals accepted.
    three_equals!(abc); // { dg-error "" "" { target *-*-* } }

    // Only equals accepted.
    three_equals!(!!); // { dg-error "" "" { target *-*-* } }

    // Only three characters expected.
    three_equals!(===a); // { dg-error "" "" { target *-*-* } }
}

