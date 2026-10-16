fn main() {
    match (1, 2) {
        (x, x) => {} // { dg-error ".E0416." "" { target *-*-* } }
    }
}

