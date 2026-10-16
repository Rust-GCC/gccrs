use std::fmt::Debug;

fn foo(x: impl Debug) -> String {
    x // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() { }

