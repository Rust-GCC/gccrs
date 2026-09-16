enum Quux<T> { Bar }

fn foo(c: Quux) { assert!((false)); } // { dg-error ".E0107." "" { target *-*-* } }

fn main() { panic!(); }

