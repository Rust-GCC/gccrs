#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
trait Sized {}

#[derive(CoercePointee)]
// { dg-error "use of unstable library feature" "" target { *-*-* } .- 1 }
// { dg-error "could not resolve trait .CoercePointee." "" target { *-*-* } .- 2 }
struct Floop<T> {
    a: *const T,
}
