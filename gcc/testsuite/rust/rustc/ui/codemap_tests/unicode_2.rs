#![feature(non_ascii_idents)]

fn main() {
    let _ = ("a̐éö̲", 0u7); // { dg-error "" "" { target *-*-* } }
    let _ = ("아あ", 1i42); // { dg-error "" "" { target *-*-* } }
    let _ = a̐é; // { dg-error ".E0425." "" { target *-*-* } }
}

