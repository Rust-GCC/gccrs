#![allow(incomplete_features)]
#![feature(generic_associated_types)]

trait Shadow<'a> {
    type Bar<'a>;
// { dg-error ".E0496." "" { target *-*-* } .-1 }
}

trait NoShadow<'a> {
    type Bar<'b>; // OK
}

impl<'a> NoShadow<'a> for &'a u32 {
    type Bar<'a> = i32;
// { dg-error ".E0496." "" { target *-*-* } .-1 }
}

trait ShadowT<T> {
    type Bar<T>;
// { dg-error ".E0403." "" { target *-*-* } .-1 }
}

trait NoShadowT<T> {
    type Bar<U>; // OK
}

impl<T> NoShadowT<T> for Option<T> {
    type Bar<T> = i32;
// { dg-error ".E0403." "" { target *-*-* } .-1 }
}

fn main() {}

