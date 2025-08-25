// run-rustfix

#![deny(unused_mut)]

fn main() {
    let mut x; // { dg-error "" "" { target *-*-* } }
    x = String::new();
    dbg!(x);
}

