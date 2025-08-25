use std::mem;

struct Misc<T:?Sized>(T);

fn check<T: Iterator, U: ?Sized>() {
    // suggest a where-clause, if needed
    mem::size_of::<U>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    mem::size_of::<Misc<U>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // ... even if T occurs as a type parameter

    <u64 as From<T>>::from;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    <u64 as From<<T as Iterator>::Item>>::from;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // ... but not if there are inference variables

    <Misc<_> as From<T>>::from;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // ... and also not if the error is not related to the type

    mem::size_of::<[T]>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    mem::size_of::<[&U]>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
}

