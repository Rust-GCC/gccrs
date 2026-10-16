fn main() {
    let value = [7u8];
    while Some(0) = value.get(0) { // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0070." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

        // FIXME The following diagnostic should also be emitted
        // HELP you might have meant to use pattern matching
    }
}

