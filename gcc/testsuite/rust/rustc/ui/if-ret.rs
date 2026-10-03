// run-pass

#![allow(unused_parens)]
// pretty-expanded FIXME #23616

fn foo() { if (return) { } } // { dg-warning "" "" { target *-*-* } }

pub fn main() { foo(); }

