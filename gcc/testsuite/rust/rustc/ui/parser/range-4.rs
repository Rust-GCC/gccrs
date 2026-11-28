// Test range syntax - syntax errors.

pub fn main() {
    let r = ..1..2;
// { dg-error "" "" { target *-*-* } .-1 }
}

