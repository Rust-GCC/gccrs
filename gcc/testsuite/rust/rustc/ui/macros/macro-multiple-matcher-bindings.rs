// Test that duplicate matcher binding names are caught at declaration time, rather than at macro
// invocation time.

#![allow(unused_macros)]

macro_rules! foo1 {
    ($a:ident, $a:ident) => {}; // { dg-error "" "" { target *-*-* } }
    ($a:ident, $a:path) => {};  // { dg-error "" "" { target *-*-* } }
}

macro_rules! foo2 {
    ($a:ident) => {}; // OK
    ($a:path) => {};  // OK
}

macro_rules! foo3 {
    ($a:ident, $($a:ident),*) => {}; // { dg-error "" "" { target *-*-* } }
    ($($a:ident)+ # $($($a:path),+);*) => {}; // { dg-error "" "" { target *-*-* } }
}

fn main() {}

