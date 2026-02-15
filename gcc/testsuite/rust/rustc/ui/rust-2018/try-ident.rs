// run-rustfix
// check-pass

#![warn(rust_2018_compatibility)]

fn main() {
    try();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn try() {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

