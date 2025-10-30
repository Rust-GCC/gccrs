// Test that we can deduce when projections like `T::Item` outlive the
// function body. Test that this does not imply that `T: 'a` holds.

// compile-flags:-Zborrowck=mir -Zverbose

use std::cell::Cell;

fn twice<F, T>(mut value: T, mut f: F)
where
    F: FnMut(&T, Cell<&Option<T::Item>>),
    T: Iterator,
{
    let mut n = value.next();
    f(&value, Cell::new(&n));
    f(&value, Cell::new(&n));
}

fn generic1<T: Iterator>(value: T) {
    // No error here:
    twice(value, |value_ref, item| invoke1(item));
}

fn invoke1<'a, T>(x: Cell<&'a Option<T>>)
where
    T: 'a,
{
}

fn generic2<T: Iterator>(value: T) {
    twice(value, |value_ref, item| invoke2(value_ref, item));
// { dg-error ".E0310." "" { target *-*-* } .-1 }
}

fn invoke2<'a, T, U>(a: &T, b: Cell<&'a Option<U>>)
where
    T: 'a,
{
}

fn main() {}

