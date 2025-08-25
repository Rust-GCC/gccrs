#![feature(crate_in_paths)]

fn main() {
    let crate = 0;
// { dg-error ".E0532." "" { target *-*-* } .-1 }
}

