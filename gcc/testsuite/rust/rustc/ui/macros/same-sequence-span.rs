// aux-build:proc_macro_sequence.rs

// Regression test for issue #62831: Check that multiple sequences with the same span in the
// left-hand side of a macro definition behave as if they had unique spans, and in particular that
// they don't crash the compiler.

#![allow(unused_macros)]

extern crate proc_macro_sequence;

// When ignoring spans, this macro has the same macro definition as `generated_foo` in
// `proc_macro_sequence.rs`.
macro_rules! manual_foo {
    (1 $x:expr $($y:tt,)*   // { dg-error "" "" { target *-*-* } }
               $(= $z:tt)*  // { dg-error "" "" { target *-*-* } }
    ) => {};
}

proc_macro_sequence::make_foo!(); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

