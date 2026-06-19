// Test that we do some basic error correction in the tokeniser (and don't ICE).

fn main() {
    if foo {
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    ) // { dg-error "" "" { target *-*-* } }
}

