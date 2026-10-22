// compile-flags: --edition 2018

#![feature(try_blocks)]

fn main() {
    let res: Option<bool> = try {
        true
    } catch { };
// { dg-error "" "" { target *-*-* } .-1 }
}

