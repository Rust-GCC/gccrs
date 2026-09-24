#![feature(concat_idents)]

fn main() {
    let x = concat_idents!(); // { dg-error "" "" { target *-*-* } }
}

