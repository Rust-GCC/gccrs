// Test for #49257:
// emits good diagnostics for `..` pattern fragments not in the last position.

#![allow(unused)]

struct Point { x: u8, y: u8 }

fn main() {
    let p = Point { x: 0, y: 0 };
    let Point { .., y, } = p; // { dg-error "" "" { target *-*-* } }
    let Point { .., y } = p; // { dg-error "" "" { target *-*-* } }
    let Point { .., } = p; // { dg-error "" "" { target *-*-* } }
    let Point { .. } = p;
}

