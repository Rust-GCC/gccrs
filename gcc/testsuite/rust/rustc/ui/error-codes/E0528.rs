fn main() {
    let r = &[1, 2];
    match r {
        &[a, b, c, rest @ ..] => {
// { dg-error ".E0528." "" { target *-*-* } .-1 }
        }
    }
}

