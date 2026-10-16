use std::fmt::Debug;

fn foo(x: impl Debug, y: impl Debug) -> String {
    let mut a = x;
    a = y; // { dg-error ".E0308." "" { target *-*-* } }
    format!("{:?}", a)
}

fn main() { }

