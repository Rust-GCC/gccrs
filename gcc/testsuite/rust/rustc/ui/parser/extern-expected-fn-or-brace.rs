// Verifies that the expected token errors for `extern crate` are raised.

extern "C" mod foo; // { dg-error "" "" { target *-*-* } }

