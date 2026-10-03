#![feature(non_ascii_idents)]
#![deny(non_ascii_idents)]

const חלודה: usize = 2; // { dg-error "" "" { target *-*-* } }

fn coöperation() {} // { dg-error "" "" { target *-*-* } }

fn main() {
    let naïveté = 2; // { dg-error "" "" { target *-*-* } }

    // using the same identifier the second time won't trigger the lint.
    println!("{}", naïveté);
}

