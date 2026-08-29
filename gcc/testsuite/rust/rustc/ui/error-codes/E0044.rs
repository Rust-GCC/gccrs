extern {
    fn sqrt<T>(f: T) -> T;
// { dg-error ".E0044." "" { target *-*-* } .-1 }
// { help ".E0044." "" { target *-*-* } .-2 }
// { dg-note ".E0044." "" { target *-*-* } .-3 }
}

fn main() {
}

