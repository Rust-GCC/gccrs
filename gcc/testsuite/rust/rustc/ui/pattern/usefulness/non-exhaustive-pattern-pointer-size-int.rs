use std::{usize, isize};

fn main() {
    match 0usize {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
// { dg-note ".E0004." "" { target *-*-* } .-2 }
// { dg-note ".E0004." "" { target *-*-* } .-3 }
// { dg-note ".E0004." "" { target *-*-* } .-4 }
        0 ..= usize::MAX => {}
    }

    match 0isize {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
// { dg-note ".E0004." "" { target *-*-* } .-2 }
// { dg-note ".E0004." "" { target *-*-* } .-3 }
// { dg-note ".E0004." "" { target *-*-* } .-4 }
        isize::MIN ..= isize::MAX => {}
    }

    match 7usize {}
// { dg-error ".E0004." "" { target *-*-* } .-1 }
// { dg-note ".E0004." "" { target *-*-* } .-2 }
}

