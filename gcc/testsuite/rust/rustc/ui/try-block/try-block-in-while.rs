// compile-flags: --edition 2018

#![feature(try_blocks)]

fn main() {
    while try { false } {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

