// compile-flags: --edition 2018

fn main() {
    let try = "foo"; // { dg-error "" "" { target *-*-* } }
}

