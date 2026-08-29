//! A test to ensure that helpful `note` messages aren't emitted more often
//! than necessary.

// check-pass

// Although there are three warnings, we should only get two "lint level defined
// here" notes pointing at the `warnings` span, one for each error type.
#![warn(unused)]


fn main() {
    let theTwo = 2; // { dg-warning "" "" { target *-*-* } }
    let theOtherTwo = 2; // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    println!("{}", theTwo);
}

