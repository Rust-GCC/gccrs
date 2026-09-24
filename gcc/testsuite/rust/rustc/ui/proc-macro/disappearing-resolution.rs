// Regression test for issue #64803 (initial attribute resolution can disappear later).

// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

mod m {
    use test_macros::Empty;
}
use m::Empty; // { dg-error ".E0603." "" { target *-*-* } }

// To resolve `empty_helper` we need to resolve `Empty`.
// During initial resolution `use m::Empty` introduces no entries, so we proceed to `macro_use`,
// successfully resolve `Empty` from there, and then resolve `empty_helper` as its helper.
// During validation `use m::Empty` introduces a `Res::Err` stub, so `Empty` resolves to it,
// and `empty_helper` can no longer be resolved.
#[empty_helper] // { dg-error "" "" { target *-*-* } }
#[derive(Empty)]
struct S;

fn main() {}

