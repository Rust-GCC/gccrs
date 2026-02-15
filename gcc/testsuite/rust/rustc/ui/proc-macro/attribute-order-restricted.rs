// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

#[identity_attr] // OK
#[derive(Clone)]
struct Before;

#[derive(Clone)]
#[identity_attr] // { dg-error "" "" { target *-*-* } }
struct After;

fn main() {}

