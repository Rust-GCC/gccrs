fn main() {
    let mut buf = &[1, 2, 3, 4];
    buf.iter_mut(); // { dg-error ".E0596." "" { target *-*-* } }
}

