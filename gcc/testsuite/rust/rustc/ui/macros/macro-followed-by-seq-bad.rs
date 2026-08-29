// Regression test for issue #25436: check that things which can be
// followed by any token also permit X* to come afterwards.

#![allow(unused_macros)]

macro_rules! foo {
  ( $a:expr $($b:tt)* ) => { }; // { dg-error "" "" { target *-*-* } }
  ( $a:ty $($b:tt)* ) => { };   // { dg-error "" "" { target *-*-* } }
}

fn main() { }

