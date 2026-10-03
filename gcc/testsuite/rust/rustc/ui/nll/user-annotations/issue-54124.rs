#![feature(nll)]

fn test<'a>() {
    let _:fn(&()) = |_:&'a ()| {}; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    test();
}

