#![allow(dead_code)]

// compile-flags: -D unused-comparisons
fn main() { }

fn foo() {
    let mut i = 100_usize;
    while i >= 0 { // { dg-error "" "" { target *-*-* } }
        i -= 1;
    }
}

fn bar() -> i8 {
    return 123;
}

fn bleh() {
    let u = 42u8;
    let _ = u > 255; // { dg-error "" "" { target *-*-* } }
    let _ = 255 < u; // { dg-error "" "" { target *-*-* } }
    let _ = u < 0; // { dg-error "" "" { target *-*-* } }
    let _ = 0 > u; // { dg-error "" "" { target *-*-* } }
    let _ = u <= 255; // { dg-error "" "" { target *-*-* } }
    let _ = 255 >= u; // { dg-error "" "" { target *-*-* } }
    let _ = u >= 0; // { dg-error "" "" { target *-*-* } }
    let _ = 0 <= u; // { dg-error "" "" { target *-*-* } }
}

