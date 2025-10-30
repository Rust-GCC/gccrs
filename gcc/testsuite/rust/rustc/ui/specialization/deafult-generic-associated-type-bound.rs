// Check that default generics associated types are validated.

#![feature(specialization)]
#![feature(generic_associated_types)]
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-2 }

trait X {
    type U<'a>: PartialEq<&'a Self> where Self: 'a;
    fn unsafe_compare<'b>(x: Option<Self::U<'b>>, y: Option<&'b Self>) {
        match (x, y) {
            (Some(a), Some(b)) => a == b,
            _ => false,
        };
    }
}

impl<T: 'static> X for T {
    default type U<'a> = &'a T;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

struct NotComparable;

pub fn main() {
    <NotComparable as X>::unsafe_compare(None, None);
}

