// aux-build:edition-lint-paths.rs
// run-rustfix
// compile-flags:--extern edition_lint_paths --cfg blandiloquence
// edition:2018

#![deny(rust_2018_idioms)]
#![allow(dead_code)]

// The suggestion span should include the attribute.

#[cfg(blandiloquence)] // { help "" "" { target *-*-* } }
extern crate edition_lint_paths;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

