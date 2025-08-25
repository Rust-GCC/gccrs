struct X {
    a: u8 /// document
// { dg-error ".E0585." "" { target *-*-* } .-1 }
// { help ".E0585." "" { target *-*-* } .-2 }
}

fn main() {
    let y = X {a: 1};
}

