// { dg-error "" "" { target *-*-* } }

// compile-flags: --target thumbv7em-none-eabihf
// needs-llvm-components: arm
#![deny(unsafe_code)]
#![deny(warnings)]
#![no_std]

extern crate cortex_m;

