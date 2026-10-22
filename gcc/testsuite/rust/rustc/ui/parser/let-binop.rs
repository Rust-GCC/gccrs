// run-rustfix

fn main() {
    let a: i8 *= 1; // { dg-error "" "" { target *-*-* } }
    let _ = a;
    let b += 1; // { dg-error "" "" { target *-*-* } }
    let _ = b;
    let c *= 1; // { dg-error "" "" { target *-*-* } }
    let _ = c;
}

