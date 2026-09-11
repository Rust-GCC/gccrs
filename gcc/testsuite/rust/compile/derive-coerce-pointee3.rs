// { dg-additional-options "-frust-compat-mode=1.84" }

#![feature(no_core)]
#![feature(lang_items)]
#![feature(derive_coerce_pointee)]
#![no_core]

#[lang = "sized"]
trait Sized {}

#[derive(CoercePointee)] // { dg-warning "no effect" }
pub struct Floop<T>(*const T);
