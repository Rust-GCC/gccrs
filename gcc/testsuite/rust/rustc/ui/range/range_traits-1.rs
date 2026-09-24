use std::ops::*;

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Debug)]
struct AllTheRanges {
    a: Range<usize>,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-5 }
// { dg-error ".E0277." "" { target *-*-* } .-6 }
    b: RangeTo<usize>,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-5 }
// { dg-error ".E0277." "" { target *-*-* } .-6 }
    c: RangeFrom<usize>,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-5 }
// { dg-error ".E0277." "" { target *-*-* } .-6 }
    d: RangeFull,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-5 }
// { dg-error ".E0277." "" { target *-*-* } .-6 }
    e: RangeInclusive<usize>,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-5 }
// { dg-error ".E0277." "" { target *-*-* } .-6 }
    f: RangeToInclusive<usize>,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-5 }
// { dg-error ".E0277." "" { target *-*-* } .-6 }
}

fn main() {}

