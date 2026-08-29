struct X {
    a: u8 /** document a */,
// { dg-error ".E0585." "" { target *-*-* } .-1 }
// { help ".E0585." "" { target *-*-* } .-2 }
}

struct Y {
    a: u8 /// document a
// { dg-error ".E0585." "" { target *-*-* } .-1 }
// { help ".E0585." "" { target *-*-* } .-2 }
}

fn main() {
    let x = X { a: 1 };
    let y = Y { a: 1 };
}

