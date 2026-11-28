fn main() {
    match Some("foo") {
        None::<isize> => {}   // { dg-error ".E0308." "" { target *-*-* } }
        Some(_) => {}
    }
}

