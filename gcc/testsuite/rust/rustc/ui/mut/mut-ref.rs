fn main() {
    let mut ref x = 10; // { dg-error "" "" { target *-*-* } }
    let ref mut y = 11;
}

