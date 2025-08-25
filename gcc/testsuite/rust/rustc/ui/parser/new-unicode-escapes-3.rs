pub fn main() {
    let s1 = "\u{d805}"; // { dg-error "" "" { target *-*-* } }
    let s2 = "\u{ffffff}"; // { dg-error "" "" { target *-*-* } }
}

