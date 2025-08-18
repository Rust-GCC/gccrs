// wasm32 does not support benches (no time).
#![cfg(not(target_arch = "wasm32"))]
#![feature(flt2dec)]
#![feature(test)]

extern crate test;

mod any;
mod ascii;
mod char;
mod fmt;
mod hash;
mod iter;
mod num;
mod ops;
mod pattern;
mod slice;
