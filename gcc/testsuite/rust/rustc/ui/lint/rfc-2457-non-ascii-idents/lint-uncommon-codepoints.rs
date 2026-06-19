#![feature(non_ascii_idents)]
#![deny(uncommon_codepoints)]

const µ: f64 = 0.000001; // { dg-error "" "" { target *-*-* } }

fn dĳkstra() {} // { dg-error "" "" { target *-*-* } }

fn main() {
    let ㇻㇲㇳ = "rust"; // { dg-error "" "" { target *-*-* } }

    // using the same identifier the second time won't trigger the lint.
    println!("{}", ㇻㇲㇳ);
}

