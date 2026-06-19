// run-pass

#![warn(unused)]

enum Enum { // { dg-warning "" "" { target *-*-* } }
    A,
    B,
    C,
    D,
}

struct Struct { // { dg-warning "" "" { target *-*-* } }
    a: usize,
    b: usize,
    c: usize,
    d: usize,
}

fn func() -> usize { // { dg-warning "" "" { target *-*-* } }
    3
}

fn
func_complete_span() // { dg-warning "" "" { target *-*-* } }
-> usize
{
    3
}

fn main() {}

