// ignore-pretty pretty-printing is unhygienic

// aux-build:intercrate.rs

#![feature(decl_macro)]

extern crate intercrate;

fn main() {
    assert_eq!(intercrate::foo::m!(), 1);
// { dg-error "" "" { target *-*-* } .-1 }
}

