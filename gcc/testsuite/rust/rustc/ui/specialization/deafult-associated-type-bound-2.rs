// Check that generic predicates are also checked for default associated types.
#![feature(specialization)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait X<T> {
    type U: PartialEq<T>;
    fn unsafe_compare(x: Option<Self::U>, y: Option<T>) {
        match (x, y) {
            (Some(a), Some(b)) => a == b,
            _ => false,
        };
    }
}

impl<B: 'static, T> X<B> for T {
    default type U = &'static B;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

pub fn main() {
    <i32 as X<i32>>::unsafe_compare(None, None);
}

