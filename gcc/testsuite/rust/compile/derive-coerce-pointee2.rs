#![feature(no_core)]
#![feature(lang_items)]
#![feature(derive_coerce_pointee)]
#![no_core]

#[lang = "sized"]
trait Sized {}

#[derive(CoercePointee)]
// { dg-error "derive(CoercePointee) requires a compatibility mode" "" target { *-*-* } .- 1 }
// { dg-error "could not resolve trait .CoercePointee." "" target { *-*-* } .- 2 }
pub struct Floop<T>(*const T);
