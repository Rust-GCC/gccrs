#![deny(safe_packed_borrows)]

// check that derive on a packed struct with non-Copy fields
// correctly. This can't be made to work perfectly because
// we can't just use the field from the struct as it might
// not be aligned.

#[derive(Copy, Clone, PartialEq, Eq)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-2 }
#[repr(packed)]
pub struct Foo<T>(T, T, T);

#[derive(PartialEq, Eq)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
#[repr(packed)]
pub struct Bar(u32, u32, u32);

#[derive(PartialEq)]
struct Y(usize);

#[derive(PartialEq)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
#[repr(packed)]
struct X(Y);

fn main() {}

