// Test the parser for the "while parsing this or-pattern..." label here.

fn main() {
    match Some(42) {
        Some(42) | .=. => {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
    }
}

