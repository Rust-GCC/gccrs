#![feature(or_patterns)]

fn main() {
    let 0 | (1 | 2) = 0; // { dg-error ".E0005." "" { target *-*-* } }
    match 0 {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        0 | (1 | 2) => {}
    }
}

