#![feature(no_core)]
#![no_core]

const NAN: f64 = 0.0 / 0.0; // { dg-warning "unused name" "" { target *-*-* } }
