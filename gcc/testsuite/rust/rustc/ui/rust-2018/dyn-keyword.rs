// edition:2015
// run-rustfix

#![allow(unused_variables)]
#![deny(keyword_idents)]

fn main() {
    let dyn = (); // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
}

