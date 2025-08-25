#![deny(unused_imports)]

struct S;

fn main() {
    use S;  // { dg-error "" "" { target *-*-* } }

    let _s = S;
}

