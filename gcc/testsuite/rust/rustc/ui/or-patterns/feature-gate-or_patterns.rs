fn main() {}

pub fn example(x: Option<usize>) {
    match x {
        Some(0 | 1 | 2) => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
        _ => {}
    }
}

// Test the `pat` macro fragment parser:
macro_rules! accept_pat {
    ($p:pat) => {}
}

accept_pat!((p | q)); // { dg-error ".E0658." "" { target *-*-* } }
accept_pat!((p | q,)); // { dg-error ".E0658." "" { target *-*-* } }
accept_pat!(TS(p | q)); // { dg-error ".E0658." "" { target *-*-* } }
accept_pat!(NS { f: p | q }); // { dg-error ".E0658." "" { target *-*-* } }
accept_pat!([p | q]); // { dg-error ".E0658." "" { target *-*-* } }

// Non-macro tests:

#[cfg(FALSE)]
fn or_patterns() {
    // Gated:

    let | A | B; // { dg-error ".E0658." "" { target *-*-* } }
    let A | B; // { dg-error ".E0658." "" { target *-*-* } }
    for | A | B in 0 {} // { dg-error ".E0658." "" { target *-*-* } }
    for A | B in 0 {} // { dg-error ".E0658." "" { target *-*-* } }
    fn fun((A | B): _) {} // { dg-error ".E0658." "" { target *-*-* } }
    let _ = |(A | B): u8| (); // { dg-error ".E0658." "" { target *-*-* } }
    let (A | B); // { dg-error ".E0658." "" { target *-*-* } }
    let (A | B,); // { dg-error ".E0658." "" { target *-*-* } }
    let A(B | C); // { dg-error ".E0658." "" { target *-*-* } }
    let E::V(B | C); // { dg-error ".E0658." "" { target *-*-* } }
    let S { f1: B | C, f2 }; // { dg-error ".E0658." "" { target *-*-* } }
    let E::V { f1: B | C, f2 }; // { dg-error ".E0658." "" { target *-*-* } }
    let [A | B]; // { dg-error ".E0658." "" { target *-*-* } }

    // Top level of `while`, `if`, and `match` arms are allowed:

    while let | A = 0 {}
    while let A | B = 0 {}
    if let | A = 0 {}
    if let A | B = 0 {}
    match 0 {
        | A => {},
        A | B => {},
    }
}

