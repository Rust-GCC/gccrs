fn main() {
    match (0, 1) {
        (, ..) => {} // { dg-error "" "" { target *-*-* } }
    }
}

