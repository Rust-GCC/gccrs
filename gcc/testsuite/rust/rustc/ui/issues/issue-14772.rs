// compile-flags: --test

#[test]
mod foo {} // { dg-error "" "" { target *-*-* } }

fn main() {}

