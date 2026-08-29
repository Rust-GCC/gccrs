fn main() {
    let x: fn~() = || (); // { dg-error "" "" { target *-*-* } }
}

