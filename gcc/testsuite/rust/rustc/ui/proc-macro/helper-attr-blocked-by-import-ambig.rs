// aux-build:test-macros.rs

#[macro_use(Empty)]
extern crate test_macros;
use test_macros::empty_attr as empty_helper;

#[derive(Empty)]
#[empty_helper] // { dg-error ".E0659." "" { target *-*-* } }
struct S;

fn main() {}

