// build-fail
// compile-flags: -C debug-assertions

#![deny(arithmetic_overflow, const_err)]

fn main() {
    let _x = 1 << -1;
// { dg-error "" "" { target *-*-* } .-1 }
}

