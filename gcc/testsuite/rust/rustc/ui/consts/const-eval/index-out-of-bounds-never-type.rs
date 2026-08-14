// build-fail

// Regression test for #66975
#![warn(const_err, unconditional_panic)]
#![feature(never_type)]

struct PrintName<T>(T);

impl<T> PrintName<T> {
    const VOID: ! = { let x = 0 * std::mem::size_of::<T>(); [][x] };
// { dg-warning "" "" { target *-*-* } .-1 }

}

fn f<T>() {
    let _ = PrintName::<T>::VOID;
// { dg-error "" "" { target *-*-* } .-1 }
}

pub fn main() {
    f::<()>();
}

