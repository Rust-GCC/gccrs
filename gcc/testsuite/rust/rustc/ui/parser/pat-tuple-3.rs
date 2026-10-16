fn main() {
    match (0, 1, 2) {
        (.., pat, ..) => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

