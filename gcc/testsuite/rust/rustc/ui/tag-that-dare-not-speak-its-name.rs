// Issue #876

use std::vec::Vec;

fn last<T>(v: Vec<&T> ) -> std::option::Option<T> {
    ::std::panic!();
}

fn main() {
    let y;
    let x : char = last(y);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}

