// aux-build:edition-lint-paths.rs
// run-rustfix

// Oddball: crate is renamed, making it harder for us to rewrite
// paths. We don't (and we leave the `extern crate` in place).

#![feature(rust_2018_preview)]
#![deny(absolute_paths_not_starting_with_crate)]

extern crate edition_lint_paths as my_crate;

use my_crate::foo;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {
    foo();
}

