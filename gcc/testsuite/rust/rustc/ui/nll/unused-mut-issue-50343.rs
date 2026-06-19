// run-rustfix

#![deny(unused_mut)]
#![allow(unused_variables)] // for rustfix

fn main() {
    vec![(42, 22)].iter().map(|(mut x, _y)| ()).count();
// { dg-error "" "" { target *-*-* } .-1 }
}

