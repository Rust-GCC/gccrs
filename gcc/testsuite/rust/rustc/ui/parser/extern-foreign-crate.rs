// Verifies that the expected token errors for `extern crate` are
// raised

extern crate foo {} // { dg-error "" "" { target *-*-* } }

