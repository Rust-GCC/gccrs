fn main() {
    let x = (0, 2);

    match x {
        (0, ref y) | (y, 0) => {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
        _ => ()
    }
}

