fn main() {
    let x = "\x80"; // { dg-error "" "" { target *-*-* } }
    let y = "\xff"; // { dg-error "" "" { target *-*-* } }
    let z = "\xe2"; // { dg-error "" "" { target *-*-* } }
    let a = b"\x00e2";  // ok because byte literal
}

